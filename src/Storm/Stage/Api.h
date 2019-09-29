#ifndef Storm_Stage_Api_h_
#define Storm_Stage_Api_h_
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

#include "Cpl/System/ElapsedTime.h"
#include "Storm/Type/EquipmentTimes.h"
#include "Storm/Type/CycleInfo.h"
#include "Storm/Type/VirtualOutputs.h"

/// 
namespace Storm {
/// 
namespace Type {


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
class Api
{
public:
	/** This method is to reset the stage to initial stage an clear out an
        'history' that the stage may have acquired over time.  The intended
        use case for this method is when/if the equipment configuration changes
        (i.e. change in the mp_systemType MP)
	 */
    virtual void reset() noexcept = 0;


public:
    /** This method is used to activate the stage (i.e. begin actively conditioning
        the air) when it is FIRST stage (of possibly many) for the current
        operating mode.

        @param pvOut        - The current capacity demand for the entire system.
        @param beginTimes   - Contains the begin On/Off times for the Indoor and 
                              Outdoor equipment.
	 */
    virtual void requestOn( float pvOut, const Storm::Type::EquipmentTimes_T& beginTimes, Storm::Type::VirtualOutputs_T& vOutputs ) noexcept = 0;

    /** This method is used to transition from current stage to the next/higher
        stage.  The method should be called on the current active stage. The
        'nextStage' is notified when it is becomes the active stage.

        @param pvOut        - The current capacity demand for the entire system.
        @param beginTimes   - Contains the begin On/Off times for the Indoor and
                              Outdoor equipment.
        @param nextStage    - Reference to 'next higher' stage to be used to
                              increase the output capacity
     */
    virtual void requestAsSupplement( float pvOut, const Storm::Type::EquipmentTimes_T& beginTimes, Storm::Type::VirtualOutputs_T& vOutputs, Api& nextStage ) noexcept = 0;

    /** This method is used to turn off (deactivate) the stage.  If the stage
        is the first stage, then the equipment is turned off.  If the stage is
        is not the first, then 'lowerStage' is notified when the lowerStage
        becomes the active stage.

        @param pvOut        - The current capacity demand for the entire system.
        @param beginTimes   - Contains the begin On/Off times for the Indoor and
                              Outdoor equipment.
        @param lowerStage   - Pointer to 'next lower' stage to be used with
                              less the output capacity.  If the 'lowerStage'
                              is null/0, then it assumed that the current stage
                              is the first stage - and the HVAC equipment is
                              turned off
     */
    virtual void requestOff( float pvOut, const Storm::Type::EquipmentTimes_T& beginTimes, Storm::Type::VirtualOutputs_T& vOutputs, Api* lowerStage=0 ) noexcept = 0;

    /** This method is used to 'reset' the Stage's internal FSM to the off
        state. The intended use for this method is turn off the stage when system 
        transitions to the off operating mode and/or operating mode that is
        not appropriate for the stage's operating mode.

        NOTE: For this scenario - the new operating mode is expected/required
              to ensure the HVAC outputs are in the proper state (i.e. clean-up
              previous the mode's outputs).
     */
    virtual void requestModeToOff( const Storm::Type::EquipmentTimes_T& beginTimes ) noexcept = 0;


public:
    /** This method is used execute/run the stage.  This method should be called
        every time that its contain Equipment/Component executes.  It should also
        be called for all stages (not just the current stage).

        @param pvOut        - The current capacity demand for the entire system.
        @param beginTimes   - Contains the begin On/Off times for the Indoor and
                              Outdoor equipment.
        @param cycleInfo    - Returns the current Cycle information.  Note: the
                              cycle information is ONLY updated if the stage
                              instance is the active stage OR if the stage 
                              instance is the 'first' stage and the first stage 
                              is in the 'Off' state (i.e. all stages off). 
     */
    virtual void execute( float pvOut, const Storm::Type::EquipmentTimes_T& beginTimes, Storm::Type::VirtualOutputs_T& vOutputs, Storm::Type::CycleInfo_T& cycleInfo ) noexcept = 0;

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

    /** This method returns true if the current stage is process of transitioning
        to being the active stage (from a lower stage).

        Note: When this method returns true, the method isActive() also returns
              true.
     */
    virtual bool isTransitioningFromLowerStage() const noexcept = 0;

    /** This method returns true if the current stage is process of transitioning
        from the active stage (to a lower stage).
    
        Note: When this method returns true, the method isActive() also returns
              true.
     */
    virtual bool isTransitioningBackToLowerStage() const noexcept = 0;


protected:
    /** This method is used to notify the next/higher stage that it is now the
        active stage
     */
    virtual void notifyAsActiveStage( float pvOut, const Storm::Type::EquipmentTimes_T& beginTimes, Storm::Type::VirtualOutputs_T& vOutputs ) noexcept = 0;

    /** This method is used to notify the previous/lower stage that it is now the 
        active stage.
     */
    virtual void notifyAsExitingSupplmenting( float pvOut, const Storm::Type::EquipmentTimes_T& beginTimes, Storm::Type::VirtualOutputs_T& vOutputs ) noexcept = 0;


public:
	/// Virtual destructor
	~Api() {}
};


};      // end namespace
}; 
#endif  // end header latch
