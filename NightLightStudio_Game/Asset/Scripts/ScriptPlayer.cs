using System;
using System.CodeDom.Compiler;
using System.Collections.Specialized;

namespace Unicorn
{
  public class ScriptPlayer : UniBehaviour
  {
    public enum State
    {
      Human = 0,
      Moth,
      Possessed
    }
    // Used player components
    RigidBody playerRB;
    // Other entity's components

    // Player Stats
    public static float humnForce = 100000.0f;  // Move force while human
    public static float mothForce = 10000.0f;  // Move force while moth
    public static float moveForce = humnForce; // Current force
    public static float maxEnergy = 15.0f;
    public static float curEnergy = maxEnergy;
    // Minimum Energy required to change to moth
    public static float EnergyTreshold = 0.0f;
    public static float EnergyDrain = 1.0f;
    public static float EnergyGain = 1.0f;
    // Move dir
    public Vector3 fwd;
    public Vector3 rht;

    // Player State
    public State CurrentState = State.Human;
    private State NextState = State.Human;
    //Vector3 inVec;

    public override void Init()
    {
      playerRB = GetRigidBody(id);
    }

    public override void LateInit()
    {
    }

    public override void Update()
    {
      Move();
      ManualStateControl();
      AutoStateControl();
      CheckChangeState();
      if (Input.GetKeyPress(VK.IMOUSE_LBUTTON))
      {
        Console.WriteLine(curEnergy);
      }
    }
    public override void FixedUpdate()
    {
    }

    public override void OnCollisionEnter(int other)
    {
    }

    public override void OnTriggerEnter(int other)
    {
    }

    public override void Exit()
    {
    }

    // Player functions
    public void Move()
    {
      // Cant move while possessed, just return
      if (CurrentState == State.Possessed)
        return;
      // Get dir vectors, assume state is moth
      fwd = ForwardVector();
      rht = Camera.GetRightVector();
      // If state is human
      if (CurrentState == State.Human)
      {
        // No y axis
        fwd.y = 0.0f;
        rht.y = 0.0f;
        fwd = fwd.normalized;
        rht = rht.normalized;
      }

      // Forward
      if (Input.GetKeyHold(VK.IKEY_W))
      Force.Apply(id, fwd, moveForce);
      // Left
      if (Input.GetKeyHold(VK.IKEY_A))
        Force.Apply(id, -rht, moveForce);
      // Backward
      if (Input.GetKeyHold(VK.IKEY_S))
        Force.Apply(id, -fwd, moveForce);
      // Right
      if (Input.GetKeyHold(VK.IKEY_D))
        Force.Apply(id, rht, moveForce);
    }

    public void ManualStateControl()
    {
      // Manual change
      if (Input.GetKeyPress(VK.IMOUSE_RBUTTON))
      {
        switch (CurrentState)
        {
          case State.Human:
            // Only can change to moth if enough energy
            if(curEnergy >= EnergyTreshold)
              NextState = State.Moth;
            break;
          case State.Moth:
            NextState = State.Human;
            break;
          case State.Possessed:
            NextState = State.Human;
            break;
        }
      }
    }

    public void AutoStateControl()
    {
      if(CurrentState != State.Human)
      {
        // Reduce energy when not in human state
        curEnergy -= EnergyDrain * RealDT();
        // Change back to human if current energy is not enough
        if(curEnergy <= 0.0f)
        {
          curEnergy = 0.0f;
          NextState = State.Human;
        }
      }
      else
      {
        // Regain energy when in human state
        curEnergy += EnergyGain * RealDT();
        // Cap at maxEnergy
        if (curEnergy >= maxEnergy)
          curEnergy = maxEnergy;
      }
    }

    public void CheckChangeState()
    {
      if (NextState != CurrentState)
      {
        CurrentState = NextState;
        Console.WriteLine(CurrentState);
        switch (CurrentState)
        {
          case State.Human:
            moveForce = humnForce;
            playerRB.isGravity = true;
            break;
          case State.Moth:
            moveForce = mothForce;
            playerRB.isGravity = false;
            break;
          case State.Possessed:
            break;
        }
      }
    }

    // Player functions
    public Vector3 ForwardVector()
    {
      return Vector3.Cross(Camera.GetUpVector(), Camera.GetRightVector());
    }
  }
}