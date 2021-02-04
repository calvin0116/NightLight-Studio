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

    public override void Init()
    {

      chnl = Audio.PlayBGM("5");
      HUD = GetCanvas(id);
      digiLogo = true;
      time = 0;

    }

    public override void LateInit()
    {
      
    }

    public override void Update()
    {

      if (digiLogo == true)
      {
        time += RealDT();
      }

      if (time > 0.5)
      {
        HUD.FindUI(0).isActive = false;
        digiLogo = false;
      }

    }

    public override void FixedUpdate()
    {
      if(Input.GetKeyPress(VK.IKEY_SPACE)|| Input.GetKeyPress(VK.IMOUSE_LBUTTON))
      {
        if (gameOver == false)
        {
          HUD.FindUI(1).isActive = false;
          HUD.FindUI(2).isActive = false;
          inGame = true;
          Audio.Stop(chnl);
        }

        else if(inGame==false && gameOver==true)
        {
          Print("next");
          //reset 
          SetNextScene("Level01_V2");
        }


      }
        
    }


    public void GameOver()
    {
      Print("Gme Over");
      HUD.FindUI(2).isActive = true;
      gameOver = true;
      inGame = false;

    }
 
    public override void Exit() { /*Console.WriteLine("Exit from C#");*/ }
  }
}