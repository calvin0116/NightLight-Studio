using System;
using System.CodeDom.Compiler;
using System.Collections.Specialized;

namespace Unicorn
{
    public class FanBehaviour : UniBehaviour
    {


        // Variables
        public bool activate;
        public int player_ID;
        public int cam_ID;
        bool isActive;

        //Required Components
        Variables ObjVariables;
        // Rigidbody

        //Variable Component Values
        public static string possessionCamPos;
        public static string possessionSpawnPos;

        // Getting Script
        ScriptPlayer script_Player;


        public override void Init()
        {
            player_ID = GameObjectFind("Player");
            cam_ID = GameObjectFind("PlayerCamera");

            ObjVariables = GetVariables(id);
            isActive = true;

            possessionCamPos = ObjVariables.GetString(0);
            possessionSpawnPos = ObjVariables.GetString(1);


        }

        public override void LateInit()
        {
            script_Player = GetScript(player_ID);

        }

        public override void Update()
        {

            if (activate == true)
            {
                // Set Player State to possessionstate
                ObjFunction();
            }



        }
        public override void FixedUpdate()
        {

        }

        public override void OnCollisionEnter(int other)
        {
            if (isActive == true)
            {
                if (other == player_ID && activate == false)
                {
                    activate = true;
                    // Set player script nextspawn position == possessionSpawnPos
                    // Set Camera script  position == possessionSpawnPos
                    // Activate energy consumption delay 


                }
            }



            if (isActive == false)
            {
                activate = false;
            }


        }

        public void Deactivate()
        {
            isActive = !isActive;
        }

        public void ObjFunction()
        {



        }

        public override void OnTriggerEnter(int other)
        {

        }

        public override void Exit()
        {

        }
    }
}
