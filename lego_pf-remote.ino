
#include "lego_pf.h"

#define IRLED   2

#define REDPOT  A0  //potentiometer for speed and direction
#define REDOVRL A1  //2 buttons for +/-max speed
#define REDOVRR A1  //2 buttons for +/-max speed
#define REDBUT  3   //stop btn
#define REDREV  4   //reversed speed
#define REDMAX  A4  //max power - controls highest speed

#define BLUPOT  A2  //potentiometer for speed and direction
#define BLUOVRL A1  //2 buttons for +/-max speed
#define BLUOVRR A1  //2 buttons for +/-max speed
#define BLUBUT  5   //stop btn
#define BLUREV  6   //reversed speed
#define BLUMAX  A5  //max power - controls highest speed

#define CHN1   7   //chn 1
#define CHN2   8   //chn 2
#define CHN3   9   //chn 3
#define CHN4   10  //chn 4

#define ADC_MIN 0
#define ADC_MAX 1023

//PF lego object on pin 2
LEGOPowerFunctions lego(IRLED);

//red chn
int redSpeed = 0;
int redSpeedM = 0;
int redA = 0;   //red analog read
int redAM = 0;  //red MAX analog read
int redSpeedL = 0;  //last values to check for a change
int redBtn = 0;
char redCancel = 0;
//blue chn
int bluSpeed = 0;
int bluSpeedM = 0;
int bluA = 0; //blue analog read
int bluAM = 0;  //red MAX analog read
int bluSpeedL = 0;  //last values to check for a change
int bluBtn = 0;
char bluCancel = 0;

//speed arrays
#define SPD_MIN 0
#define SPD_MAX 7
int fwdSpeed[] = {PWM_FLT, PWM_FWD1, PWM_FWD2, PWM_FWD3, PWM_FWD4, PWM_FWD5, PWM_FWD6, PWM_FWD7};
int revSpeed[] = {PWM_FLT, PWM_REV1, PWM_REV2, PWM_REV3, PWM_REV4, PWM_REV5, PWM_REV6, PWM_REV7};

void setup()
{
  Serial.begin(115200);
  while (!Serial)
    delay (500); //delay for Leonardo
  //pins setup
  pinMode (REDPOT, INPUT);
  pinMode (REDOVRL, INPUT);
  pinMode (REDOVRR, INPUT);
  pinMode (REDMAX, INPUT);
  pinMode (REDBUT, INPUT_PULLUP);
  pinMode (REDREV, INPUT_PULLUP);
  //
  pinMode (BLUPOT, INPUT);
  pinMode (BLUOVRL, INPUT);
  pinMode (BLUOVRR, INPUT);
  pinMode (BLUMAX, INPUT);
  pinMode (BLUBUT, INPUT_PULLUP);
  pinMode (BLUREV, INPUT_PULLUP);
  //
  pinMode (CHN1, INPUT_PULLUP);
  pinMode (CHN2, INPUT_PULLUP);
  pinMode (CHN3, INPUT_PULLUP);
  pinMode (CHN4, INPUT_PULLUP);
}

void redPWMCmdSend (int spd)
{
  //todo: read the channel
  Serial.print("RED PWM ");
  Serial.print(spd);
  Serial.print(" >> ");
  //check speed reversed/normal
  if (digitalRead(REDREV) == 0)
    spd = -spd;
  //
  if (spd < 0)
    spd = revSpeed[-spd];
  else
    spd = fwdSpeed[spd];
  //
  Serial.print(spd);
  Serial.print(" chn ");
  //
  if (digitalRead(CHN1) == 0)
  {
    lego.SingleOutput(PWM, spd, RED, CH1);
    Serial.print("1");
  }
  if (digitalRead(CHN2) == 0)
  {
    lego.SingleOutput(PWM, spd, RED, CH2);
    Serial.print("2");
  }
  if (digitalRead(CHN3) == 0)
  {
    lego.SingleOutput(PWM, spd, RED, CH3);
    Serial.print("3");
  }
  if (digitalRead(CHN4) == 0)
  {
    lego.SingleOutput(PWM, spd, RED, CH4);
    Serial.print("4");
  }
  Serial.println("");
}

void bluPWMCmdSend (int spd)
{
  //read the channel
  //todo: read the channel
  Serial.print("BLUE PWM ");
  Serial.print(spd);
  Serial.print(" >> ");
  //check speed reversed/normal
  if (digitalRead(BLUREV) == 0)
    spd = -spd;
  //
  if (spd < 0)
    spd = revSpeed[-spd];
  else
    spd = fwdSpeed[spd];
  //
  Serial.print(spd);
  Serial.print(" chn ");
  //
  if (digitalRead(CHN1) == 0)
  {
    lego.SingleOutput(PWM, spd, BLUE, CH1);
    Serial.print("1");
  }
  if (digitalRead(CHN2) == 0)
  {
    lego.SingleOutput(PWM, spd, BLUE, CH2);
    Serial.print("2");
  }
  if (digitalRead(CHN3) == 0)
  {
    lego.SingleOutput(PWM, spd, BLUE, CH3);
    Serial.print("3");
  }
  if (digitalRead(CHN4) == 0)
  {
    lego.SingleOutput(PWM, spd, BLUE, CH4);
    Serial.print("4");
  }
  Serial.println("");
}

//int redOvrLA, redOvrRA, bluOvrLA, bluOvrRA;
int ovrl;
void loop()
{
  char ic = 0; //input change flag
  redA = analogRead(REDPOT);
  redAM = analogRead(REDMAX);
  bluA = analogRead(BLUPOT);
  bluAM = analogRead(BLUMAX);
  redBtn = digitalRead(REDBUT);
  bluBtn = digitalRead(BLUBUT);
  ovrl = analogRead(REDOVRL)/100; //we use very different resistors such that we get: 0 > 8 > 6 > 3 > 5
  //redOvrRA = analogRead(REDOVRR);
  //bluOvrLA = analogRead(BLUOVRL);
  //bluOvrRA = analogRead(BLUOVRR);
  //compute red speed based on inputs
  //todo: add pot for max speeds
  redSpeedM = map(redAM, ADC_MIN, ADC_MAX, SPD_MIN, SPD_MAX);
  redSpeed = map(redA, ADC_MIN, ADC_MAX, -redSpeedM, redSpeedM);
  bluSpeedM = map(bluAM, ADC_MIN, ADC_MAX, SPD_MIN, SPD_MAX);
  bluSpeed = map(bluA, ADC_MIN, ADC_MAX, -bluSpeedM, bluSpeedM);
  //
  if (1)
  {
    Serial.print("RED pot ");
    Serial.print(redA);
    Serial.print(" btn ");
    Serial.print(redBtn);
    Serial.print(" max ");
    Serial.print(redAM);
    Serial.print(" BLU pot ");
    Serial.print(bluA);
    Serial.print(" btn ");
    Serial.print(bluBtn);
    Serial.print(" max ");
    Serial.print(bluAM);
    Serial.print(" OVR ");
    Serial.print(ovrl);
    Serial.print(" RED speed: ");
    Serial.print(redSpeed);
    Serial.print(" BLU speed: ");
    Serial.println(bluSpeed);
  }
  //
  //lego.SingleOutput( PWM, PWM_FWD5, RED, CH1);
  //RED
  if (redBtn == 0) //pressed
  {
    //lego.SingleOutput( PWM, revSpeed[0], RED, CH1);
    redPWMCmdSend(0);
    redSpeedL = redSpeed;
    redCancel = 1;
  }
  //red left/right override
  if (ovrl == 6 || ovrl == 8)
  {
    redPWMCmdSend(ovrl==6?SPD_MAX:-SPD_MAX);
  }
  //
  else if (redSpeed != redSpeedL)
  {
    if (redCancel) //wait until we get to pos 0
    {
      if (redSpeed == 0)
        redCancel = 0;
    }
    else
    {
      ic++;
      redPWMCmdSend(redSpeed);
      /*
      if (redSpeed < 0)
        //lego.SingleOutput( PWM, revSpeed[-redSpeed], RED, CH1);
        //redPWMCmdSend(revSpeed[-redSpeed]);
      else
        //lego.SingleOutput( PWM, fwdSpeed[redSpeed], RED, CH1);
        redPWMCmdSend(fwdSpeed[redSpeed]);
      */
      redSpeedL = redSpeed;
    }
  }
  //BLUE
  if (bluBtn == 0) //pressed
  {
    //lego.SingleOutput( PWM, revSpeed[0], BLUE, CH1);
    bluPWMCmdSend(0);
    bluSpeedL = bluSpeed;
    bluCancel = 1;
  }
  //blue left/right override
  if (ovrl == 3 || ovrl == 5)
  {
    bluPWMCmdSend(ovrl==3?SPD_MAX:-SPD_MAX);
  }
  //
  else if (bluSpeedL != bluSpeed)
  {
    if (bluCancel) //wait until we get to pos 0
    {
      if (bluSpeed == 0)
        bluCancel = 0;
    }
    else
    {
      ic++;
      bluPWMCmdSend(bluSpeed);
      /*
      if (bluSpeed < 0)
        //lego.SingleOutput( PWM, revSpeed[-bluSpeed], BLUE, CH1);
        bluPWMCmdSend(revSpeed[-bluSpeed]);
      else
        //lego.SingleOutput( PWM, fwdSpeed[bluSpeed], BLUE, CH1);
        bluPWMCmdSend(fwdSpeed[bluSpeed]);
      */
      bluSpeedL = bluSpeed;
    }
  }
  //some delay so we can react fast
  delay(100);
}
