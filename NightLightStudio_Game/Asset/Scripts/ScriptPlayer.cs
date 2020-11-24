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
    Variables playerVar;
    // Other entity's components

    // Player Stats, default values
    public static float humnForce = 100000.0f;  // Move force while human
    public static float mothForce = 10000.0f;  // Move force while moth
    public static float maxEnergy = 15.0f;
    public static float moveForce = humnForce; // Current force
    public static float curEnergy = maxEnergy;
    // Minimum Energy required to change to moth
    public static float EnergyTreshold = 0.0f;
    public static float EnergyDrain = 1.0f;
    public static float EnergyGain = 1.0f;
    // Move dir for camera to player
    public Vector3 fwd;
    public Vector3 rht;
    Vector3 moveDir = new Vector3(0.0f, 0.0f, 0.0f);
    bool movedX = false;
    bool movedY = false;

    // Player State
    public State CurrentState = State.Human;
    private State NextState = State.Human;
    //Vector3 inVec;

    public override void Init()
    {
      playerRB = GetRigidBody(id);
      playerVar = GetVariables(id);
      // Change default values from variables
      humnForce = playerVar.getFloat(0);
      mothForce = playerVar.getFloat(1);
      maxEnergy = playerVar.getFloat(2);
      curEnergy = maxEnergy;
      moveForce = humnForce;
    }

    public override void LateInit()
    {
      Console.WriteLine(playerVar.getFloat(0));
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

      // Reset values
      moveDir = new Vector3(0.0f, 0.0f, 0.0f);
      movedX = false;
      movedY = false;
      if (Input.GetKeyHold(VK.IKEY_W))
      {
        moveDir += fwd;
        movedY = !movedY;
        //Force.Apply(id, fwd, moveForce);
      }
      // Left
      if (Input.GetKeyHold(VK.IKEY_A))
      {
        moveDir += -rht;
        movedX = !movedX;
        //Force.Apply(id, -rht, moveForce);
      }
      // Backward
      if (Input.GetKeyHold(VK.IKEY_S))
      {
        moveDir += -fwd;
        movedY = !movedY;
        //Force.Apply(id, -fwd, moveForce);
      }
      // Right
      if (Input.GetKeyHold(VK.IKEY_D))
      {
        moveDir += rht;
        movedX = !movedX;
        //Force.Apply(id, rht, moveForce);
      }
      if (movedX || movedY)
      {
        Console.WriteLine("Applying");
        Force.Apply(id, moveDir.normalized, moveForce);
      }
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