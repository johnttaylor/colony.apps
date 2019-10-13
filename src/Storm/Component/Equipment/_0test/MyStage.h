#ifndef MyStage_h_
#define MyStage_h_
/*-----------------------------------------------------------------------------
* This file is part of the Colony.Core Project.  The Colony.Core Project is an
* open source project with a BSD type of licensing agreement.  See the license
* agreement (license.txt) in the top/ directory or on the Internet at
* http://integerfox.com/colony.core/license.txt
*
* Copyright (c) 2014-2019  John T. Taylor
*
* Redistributions of the source code must retain the above copyright notice.
*----------------------------------------------------------------------------*/


#include "Storm/Component/Equipment/Stage.h"


class MyStage : public Storm::Component::Equipment::Stage
{
public:
    int m_countReset;
    int m_countRequestOn;
    int m_countRequestAsSupplement;
    int m_countRequestOff;
    int m_countRequestOffWithLowerStage;
    int m_countRequestModeToOff;
    int m_countExecute;
    int m_countNotifyAsActiveStage;
    int m_countNotifyAsExitingSupplmenting;
    bool m_isActive;
    bool m_isSupplementing;
    bool m_isOff;
    bool m_isTransitioningFromLowerStage;
    bool m_isTransitioningBackToLowerStage;
    bool m_isOnCycle;
    bool m_isOffCycle;

public:
    MyStage()
        : m_countReset( 0 )
        , m_countRequestOn( 0 )
        , m_countRequestAsSupplement( 0 )
        , m_countRequestOff( 0 )
        , m_countRequestOffWithLowerStage( 0 )
        , m_countRequestModeToOff( 0 )
        , m_countExecute( 0 )
        , m_countNotifyAsActiveStage( 0 )
        , m_countNotifyAsExitingSupplmenting( 0 )
    {
    }

public:
    void reset() noexcept
    {
        m_countReset++;
    }

    void requestOn( Storm::Component::Control::Equipment::Args_T& args, bool startInOnCycle=true ) noexcept
    {
        m_countRequestOn++;
    }

    void requestAsSupplement( Storm::Component::Control::Equipment::Args_T& args, Stage& nextStage, bool startNextStageInOnCycle=true ) noexcept
    {
        m_countRequestAsSupplement++;
    }

    void requestOff( Storm::Component::Control::Equipment::Args_T& args, Stage* lowerStage=0, bool startLowerStageInOnCycle=true ) noexcept
    {
        m_countRequestOff++;
        if ( lowerStage )
        {
            m_countRequestOffWithLowerStage++;
        }
    }

    void requestModeToOff( Storm::Component::Control::Equipment::Args_T& args ) noexcept
    {
        m_countRequestModeToOff++;
    }

    void execute( Storm::Component::Control::Equipment::Args_T& args ) noexcept
    {
        m_countExecute++;
    }

    bool isActive() const noexcept
    {
        return m_isActive;
    }

    bool isSupplementing() const noexcept
    {
        return m_isSupplementing;
    }

    bool isOff() const noexcept
    {
        return m_isOff;
    }

    bool isTransitioningFromLowerStage() const noexcept
    {
        return m_isTransitioningFromLowerStage;
    }

    bool isTransitioningBackToLowerStage() const noexcept
    {
        return m_isTransitioningBackToLowerStage;
    }

    bool isOnCycle() const noexcept
    {
        return m_isOnCycle;
    }

    bool isOffCycle() const noexcept
    {
        return m_isOffCycle;
    }

    void notifyAsActiveStage( Storm::Component::Control::Equipment::Args_T& args, bool startInOnCycle = true ) noexcept
    {
        m_countNotifyAsActiveStage++;
    }

    void notifyAsExitingSupplmenting( Storm::Component::Control::Equipment::Args_T& args, bool startInOnCycle = true ) noexcept
    {
        m_isTransitioningBackToLowerStage++;
    }

};

#endif  // end header latch