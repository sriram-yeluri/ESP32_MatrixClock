
v2.0 Stable ✅ (Current)
We've achieved:
ESP32 Core 3.3.x compatibility
Modular architecture
Reliable Wi-Fi
Reliable NTP
Page Manager
Message Manager
Settings
Stable build
Clean separation of modules

This is our baseline.



Phase 1 — Display Engine 2.0 ⭐ (next)
This is where we'll spend our effort.
1. Event-driven page transitions
Right now the PageManager advances pages based on elapsed time.
I'd change it to this:

Message starts
      │
      ▼
Animation running
      │
      ▼
Animation complete?
      │
      ├── No → continue
      │
      └── Yes
             │
             ▼
Hold page (optional)
             │
             ▼
Next page

Benefits
No blank screens
No interrupted animations
Smooth transitions
More professional feel

Phase 2 — Dedicated Clock Renderer
Instead of using MD_Parola animations for the clock:

We'll create a dedicated renderer.
Features:
Static clock
Blinking colon
Smooth one-second updates
No flicker
Optional seconds later
This renderer won't use the scrolling animation engine at all.

Phase 3 — Font Engine
This is something I've wanted to improve since the beginning.
Currently: Display

Instead:
FontManager

├── Small
├── Medium
├── Large
├── Thin
├── Bold
├── Digital

Then every page simply requests:
display.setFont(Font::Digital);
or
display.setFont(Font::Small);

This makes future customization much easier.

Phase 4 — Page Effects
Each page can have its own entry/exit effects.
For example:
Page	Effect
Time	Static
Date	Scroll Left
Day	Scroll Up
Message	Scroll Left
Warning	Blink
Error	Invert
Eventually these can be configured from the web UI.

Phase 5 — Display Scheduler
Instead of having PageManager and Display operate independently, introduce a DisplayScheduler that coordinates them.
DisplayScheduler
│
├── PageManager
├── ClockRenderer
├── AnimationRenderer
├── FontManager
└── BrightnessManager
The scheduler becomes the single place responsible for:
selecting the current page,
starting animations,
waiting for animations to finish,
holding static pages,
then advancing.
This removes timing conflicts between modules.

Proposed milestones
v2.1 — Display Engine
Event-driven transitions
Dedicated clock renderer
Smooth page changes

v2.2 — Font System
FontManager
Digital clock font
Multiple font styles
Per-page font selection

v2.3 — Web Interface
Message editor
Brightness
Page order
Page durations
Font selection
Effects
OTA updates


Each class should have one clear responsibility.
For example:
ClockService → Time only
ClockNetwork → Networking only
MessageManager → Message storage only
PageManager → Page sequencing only
DisplayScheduler → Orchestrates what is shown and when
DisplayRenderer → Draws pixels and animations
FontManager → Typography
Settings → Configuration only

### Coding standards
Classes: PascalCase (DisplayScheduler)
Methods: camelCase (updateDisplay())
Member variables: m_ prefix (m_currentPage)
Constants: k prefix (kDefaultBrightness)
Enums: enum class only
One class per .h/.cpp pair
No logic in .ino beyond initialization and the main update loop
