#ifndef Storm_Component_Equipment_Stage_h_
#define Storm_Component_Equipment_Stage_h_
/*-----------------------------------------------------------------------------
* This file is part of the Colony.Apps Project.  The Colony.Apps Project is an
* open source project with a BSD type of licensing agreement.  See the license
* agreement (license.txt) in the top/ directory or on the Internet at
* http://integerfox.com/colony.apps/license.txt
*
* Copyright (c) 2015 - 2019 John T. Taylor
*
* Redistributions of the source code must retain the above copyright notice.
*----------------------------------------------------------------------------*/
/** @file */

#include "Storm/Component/Control.h"

/// 
namespace Storm {
/// 
namespace Component {
/// 
namespace Equipment {

/** This abstract class define 'equipments' interface to a "stage".   The "stage"
    is responsible for determine what HVAC outputs are on based on the current
    loading.
 

    if ( systemWasOff && lv > 0 )
        stage1.requestOn( normalizePvOutForStage, beginEquipmentTimes ) // stageN ->evOnRequest
    else if ( systemOn and lv > stage2Threshold )
        stage1.requestAsSupplement(normalizePvOutForStage, beginEquipmentTimes, NotifyStage& stage2 );  stageN->evNeedMoreCapacity
    else if ( lv < stage1ReturnThreshold and stage2.isActiveStage() )
        stage2.requestOff(normalizePvOutForStage, beginEquipmentTimes, NotifyStage* = stage1 ); // stageN->evExcessCapacity + isBeingSupplmentted:=true 
    else if ( systemOn and lv == 0 )
        stage1.requestOff(normalizePvOutForStage, beginEquipmentTimes, NotifyStage* = 0);       // stageN->evExcessCapacity + isBeingSupplmentted:=false (since when NotifyStage == 0) 
    
    stage1.requestModeToOff(beginEquipmentTimes);   // evOffRequest

    stage1.execute(normalizePvOutForStage, beginEquipmentTimes);
    stage2.execute(normalizePvOutForStage, beginEquipmentTimes );
    stage3.execute(normalizePvOutForStage, beginEquipmentTimes);

    stageN.isActiveStage();                     // state==Cycling|TransitioningFromLowerStage|TransitionBackToLowerStage
    stageN.isSupplementing();                   // state==SupplmentingNextStage
    stageN.isTransitioningFromLowerStage();     // state==TransitioningFromLowerStage
    stageN.isTransitioningBackToLowerStage();   // state==TransitionBackToLowerStage
    stageN.isOff();                             // state==Off

    stageN.notifyAsActiveStage(normalizePvOutForStage, beginEquipmentTimes);           // called from EnterSupplementing(). stageN+1->evOnRequest + isBeingSupplmentted:=true
    stageN.notifyAsExitingSupplmenting(normalizePvOutForStage, beginEquipmentTimes);   // called from notifyLowerStage(). -->stageN-1 ->evLessCapacityNeeded
 */
class Stage
{
public:
    /** This method is used to activate the stage (i.e. begin actively conditioning
        the air) when it is FIRST stage (of possibly many) for the current
        operating mode.
     */
    virtual void requestOn( Storm::Component::Control::Equipment::Args_T& args, bool startInOnCycle=true ) noexcept = 0;

    /** This method is used to transition from current stage to the next/higher
        stage.  The method should be called on the current active stage. The
        'nextStage' is notified when it is becomes the active stage.
     */
    virtual void requestAsSupplement( Storm::Component::Control::Equipment::Args_T& args, Stage& nextStage, bool startNextStageInOnCycle=true ) noexcept = 0;

    /** This method is used to turn off (deactivate) the stage.  If the stage
        is the first stage, then the equipment is turned off.  If the stage is
        is not the first, then previous active stage is notified when it 
        becomes the active stage.

     */                      
    virtual void requestOff( Storm::Component::Control::Equipment::Args_T& args, bool startLowerStageInOnCycle=true ) noexcept = 0;

    /** This method is used to 'reset' the Stage's internal FSM to the off
        state. The intended use for this method is turn off the stage when system 
        transitions to the off operating mode and/or to an operating mode that is
        not appropriate for the stage's operating mode.

        NOTE: For this scenario - the new operating mode is expected/required
              to ensure the HVAC outputs are in the proper state (i.e. clean-up
              previous the mode's outputs) - so that stage being requested to
              OFF does not set/clear any HVAC outputs.
     */
    virtual void requestModeToOff( Storm::Component::Control::Equipment::Args_T& args ) noexcept = 0;


public:
    /** This method is used execute/run the stage.  This method should be called
        every time that its contain Equipment/Component executes.  It should also
        be called for all stages (not just the current stage).
     */
    virtual void execute( Storm::Component::Control::Equipment::Args_T& args ) noexcept = 0;

public:
    /** This method returns true if the stage is providing capacity and is 
        consider to be the active/highest operating stage. 
        
        Note: The return value is independent of whether the stage is in On or 
        Off Cycle (i.e. if the stage is active/highest stage and current 
        executing on off cycle the method returns true).
     */
    virtual bool isActive() const noexcept = 0;

    /** This method returns true if the stage is providing capacity but IS NOT
        highest operating stage, i.e. it is supplementing higher stage(s).
     */
    virtual bool isSupplementing() const noexcept = 0;

    /** This method returns true if the stage is in the off state, i.e. the
        stage is NOT being used in any way to provide capacity.  
        
        Note: This method does NOT report the on/off cycle state/status of
              stage.
     */
    virtual bool isOff() const noexcept = 0;

    /** This method returns true if the current stage is in the process of 
        transitioning to being the active stage (from a lower stage).

        Note: When this method returns true, the method isActive() also returns
              true.
     */
    virtual bool isTransitioningFromLowerStage() const noexcept = 0;

    /** This method returns true if the current stage is in the process of 
        transitioning from the active stage (to a lower stage).
    
        Note: When this method returns true, the method isActive() also returns
              true.
     */
    virtual bool isTransitioningBackToLowerStage() const noexcept = 0;

    /** This method returns true if the current stage is in an on-cycle

        Note: When this method returns true, the method isActive() returns true 
              and the isTransitioningXxxx() methods return false;
     */
    virtual bool isOnCycle() const noexcept = 0;

    /** This method returns true if the current stage is in an off-cycle

        Note: When this method returns true, the method isActive() returns true
              and the isTransitioningXxxx() methods return false;
     */
    virtual bool isOffCycle() const noexcept = 0;


public:
    /** This method has PACKAGE Scope, i.e. it is intended to be ONLY accessible
        by other classes in the Storm::Component::Equipment namespace.  The 
        Application should NEVER call this method.

        This method is used to notify the next/higher stage that it is now the
        active stage
     */
    virtual void notifyAsActiveStage_( Storm::Component::Control::Equipment::Args_T& args, Stage& previousStage, bool startInOnCycle ) noexcept = 0;

    /** This method has PACKAGE Scope, i.e. it is intended to be ONLY accessible
        by other classes in the Storm::Component::Equipment namespace.  The
        Application should NEVER call this method.

        This method is used to notify the previous/lower stage that it is now the
        active stage.
     */
    virtual void notifyAsExitingSupplmenting_( Storm::Component::Control::Equipment::Args_T& args, bool startInOnCycle  ) noexcept = 0;


public:
	/// Virtual destructor
	~Stage() {}
};


};      // end namespace
};
};
#endif  // end header latch