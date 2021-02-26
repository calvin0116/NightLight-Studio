using System;
using System.CodeDom.Compiler;
using System.Collections.Specialized;

namespace Unicorn
{
  public class CanvasMenu : UniBehaviour
  {
    int chnl;
    bool digiLogo;
    Canvas HUD;
    bool gameOver;
    bool inGame;
    float time;
    public int player_ID;

    ScriptPlayer script_Player;
    
    //Colour Control
    float logo_Colour;
    //SFX 
    public bool vignette;
    float transitionRate;
    bool hurtUI;

    public override void Init()
    {
      player_ID = GameObjectFind("Player");
      chnl = Audio.PlayBGM("5");
      HUD = GetCanvas(id);
      digiLogo = true;
      time = 0;

      HUD.FindUI("Vignette").isActive = false;
      HUD.FindUI("DigipenLogo").SetColour(new Vector3(0.0f, 0.0f, 0.0f) );

    }

    public override void LateInit()
    {
      script_Player = GetScript(player_ID);
    }

    public override void Update()
    {

      if (digiLogo == true)
      {
        logo_Colour += DT()*1.2f;
        HUD.FindUI("DigipenLogo").SetColour(new Vector3(logo_Colour, logo_Colour, logo_Colour));
        //HUD.FindUI("DigipenLogo").alpha += 0.2f;
        time += RealDT();
      }

////////------------------------------------- Intro/Title-------------------------------------------------------------//////////////////////
      if (time > 3)
      {
        //logo_Colour = 2.0f;
        logo_Colour -= DT() * 5f;
        HUD.FindUI("DigipenLogo").SetColour(new Vector3(logo_Colour, logo_Colour, logo_Colour));

        ///Print(logo_Colour.ToString);

        if (logo_Colour < 0)
        {
          HUD.FindUI("DigipenLogo").alpha -= 0.02f;

          if (HUD.FindUI("DigipenLogo").alpha < 0)
          {
            HUD.FindUI("DigipenLogo").isActive = false;
            digiLogo = false;
          }

        }

      }

      ///////--------------------------------- Vignette Timer -----------------------------------------------------------////////////////////////
      ///
      if (vignette == true)
      {
        HUD.FindUI("Vignette").isActive = true;
      //  Print( HUD.FindUI("Vignette").alpha.ToString());
       // Print(HUD.FindUI("Vignette").isActive.ToString());
        HUD.FindUI("Vignette").alpha += transitionRate;

        if (HUD.FindUI("Vignette").alpha > 1.0f)
        {
          HUD.FindUI("Vignette").alpha = 1.0f;
        }
      }

      else
      {
        HUD.FindUI("Vignette").alpha -= 0.02f;

        if (HUD.FindUI("Vignette").alpha < 0.0f)
        {
         // HUD.FindUI("Vignette").isActive = false;
          HUD.FindUI("Vignette").alpha = 0.0f;
        }

      }


    }

    public void Dying()
    {
      HUD.FindUI("Vignette").SetColour(new Vector3(1.0f, 1.0f, 1.0f));
      transitionRate = 0.02f;
      HUD.FindUI("Vignette").alpha = 0.0f;
      vignette = true;
      hurtUI = true;
    }

    public void Safe()
    {   
      //HUD.FindUI("Vignette").alpha = 1.0f;
      vignette = false;
      hurtUI = false;
    }

    public override void FixedUpdate()
    {

      if (script_Player.activating)
      {
        vignette = true;
        HUD.FindUI("Vignette").SetColour(new Vector3(0, 0.5f, 1.0f));
        transitionRate = 0.02f;
        
      }
      else if (!script_Player.activating && !hurtUI )
      {
        vignette = false;
      
      }

      if(Input.GetKeyPress(VK.IKEY_SPACE)|| Input.GetKeyPress(VK.IMOUSE_LBUTTON))
      {
        if (gameOver == false)
        {
          HUD.FindUI("MainMenu").isActive = false;
          HUD.FindUI("EndGame").isActive = false;
          inGame = true;
          Audio.Stop(chnl);
        }

        else if(inGame==false && gameOver==true)
        {
          //Print("next");
          //reset 
          SetNextScene("Level01_V2");
        }


      }
        
    }


    public void GameOver()
    {
     //Print("Gme Over");
      HUD.FindUI("EndGame").isActive = true;
      gameOver = true;
      inGame = false;

    }
 
    public override void Exit() { /*Console.WriteLine("Exit from C#");*/ }
  }
}