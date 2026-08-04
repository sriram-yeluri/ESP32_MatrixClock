/******************************************************************************
 * Project      : ESP32 Matrix Clock Professional
 *
 * File         : MessageManager.h
 *
 * Description:
 * Scrolling message management.
 ******************************************************************************/

#ifndef MESSAGE_MANAGER_H
#define MESSAGE_MANAGER_H


#include <Arduino.h>


#include "Types.h"
#include "Constants.h"



struct DisplayMessage
{

    char text[
        Constants::Buffer::MessageSize
    ];


    bool enabled;


    uint16_t duration;


    MessageType type;

};




class MessageManager
{

public:


    MessageManager();



    /*
     * Initialize messages.
     */
    void begin();



    /*
     * Background update.
     */
    void update();



    /*
     * Current message.
     */
    const char* current() const;



    /*
     * Move next message.
     */
    void next();



    /*
     * Add new message.
     */
    bool add(

        const char* text,

        uint16_t seconds =
            Constants::Message::DefaultDuration

    );



    /*
     * Remove all.
     */
    void clear();



    uint8_t count() const;



private:


    void selectNext();



    DisplayMessage m_messages[
        Constants::Limits::MaximumMessages
    ];



    uint8_t m_count;


    uint8_t m_current;


    uint32_t m_lastChange;



};



#endif