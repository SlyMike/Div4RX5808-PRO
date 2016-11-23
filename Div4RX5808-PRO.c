/*****************************************************************************
 Dual 5.8ghz Video Receiver Diversity Controller with RX5808-PRO functionality.
 "Div4RX5808-PRO"
 Mike Dalton 2015

 RC VIDEO RECEIVER SOFTWARE FOR ATMEGA328.

 RX5808-PRO SUPPORT!
 LIVE VIDEO / SPECTRUM ANALYSER SWITCHING VIA PUSH BUTTON!
 SINGLE RX OR DIVERSITY MODES VIA PUSH BUTTON!
 SERIAL DEBUG MODE VIA PUSH BUTTON!
 MODE LED INDICATORS!
 SOFTWARE SWITCH DE-BOUNCING!
 VARIABLE SOFTWARE RSSI SMOOTHING!
 RSSI VALUES VIA 4 LEDS!
 VARIABLE RX SWITCHING RATE!
 AUTO RSSI CALIBRATION!


 Physical pins used:

 A0      RSSI1_ADC_PIN           ANALOGUE INPUT    left channel RSSI input (RX1, 0.5V-1.2V).
 A1      RSSI2_ADC_PIN           ANALOGUE INPUT    right channel RSSI input (RX2, 0.5V-1.2V).
 A2      N/A                     N/A               N/A
 A3      N/A                     N/A               N/A
 A4      N/A                     N/A               N/A
 A5      N/A                     N/A               N/A
 A6      N/A                     N/A               N/A
 A7      N/A                     N/A               N/A
 D0      SERIAL                  TX                Serial connection
 D1      SERIAL                  RX                Serial connection
 D2      MODE_SWITCH             INPUT PULLUP      Switch 1, Mode select between left channel, right channel and diversity using RSSI. (ENTERS DEBUG MODE AT POWER UP)
 D3      VIDEO_SWITCH            INPUT PULLUP      Switch 2, selects between live video output from selected receiver and ATV (spectrum analyser) output from RX8505-PRO board.
 D4      RX_CONTROL_PIN          OUTPUT            Pin to control switching between receiver 1 and 2, default LOW - RX1.
 D5      VIDEO_CONTROL_PIN       OUTPUT            Pin to control switching between live video and ATV. (ATV = Arduino TV out - SPECTRUM ANALYSER)
 D6      LED_RX_1                OUTPUT            LED for RX1 radio.
 D7      LED_RX_2                OUTPUT            LED for RX2 radio.
 D8      LED_025_P               OUTPUT            RED LED to display RSSI between 0%-25%.
 D9      LED_050_P               OUTPUT            AMBER LED to display RSSI between 26$-50%.
 D10     LED_075_P               OUTPUT            AMBER LED to display RSSI between 51%-75%.
 D11     LED_100_P               OUTPUT            GREEN LED to display RSSI between 76%-100%
 D12     LED_DIVERSITY           OUTPUT            GREEN LED to display if Diversity is Enabled.
 D13     LED_HEARTBEAT           OUTPUT            HEART BEAT LED
 AREF    (OPTION)                (INPUT)           (OPTIONAL VOLTAGE REFERENCE TO INCREASE RESOLUTION FOR RX THAT USE RSSI >1.1V).


 The main settings to change the behaviour of the unit:

 BUTTON_DEBOUNCE_MILLIS          Alters repeat speed of push buttons.
 MAX_AVERAGE_READINGS            Smoothing value for taking average RSSI readings.
 RSSI_HYSTERESIS                 Overhead for RSSI signal (RX switching) in diversity mode.
 DIVERSITY_INTERVAL_MILLIS       Minimum value, in milliseconds to toggle receivers.

 RSSI1Min, RSSI1Max              The default expected ADC readings for your RX.
 RSSI2Min, RSSI2Max              The default expected ADC readings for your RX.

 CALIB_TIMEOUT_MILLIS            Time between starting Auto Calibration and giving up.
 ONE_TIME_WARMUP_DELAY           The amount of time you expect your TX gear (RC model) to take to settle in after power up.
 *******************************************************************************/



/* Physical pin assignments. */
#define RSSI1_ADC_PIN A0                                /* ADC connected to reciever 1 RSSI output pin. */
#define RSSI2_ADC_PIN A1                                /* ADC connected to reciever 2 RSSI output pin. */
#define MODE_SWITCH 2                                   /* Press this button to switch between available RX modes. */
#define VIDEO_SWITCH 3                                  /* Press this button to switch between live video and spectrum analyser. */
#define RX_CONTROL_PIN 4                                /* Toggled by software to select RX unit. */
#define VIDEO_CONTROL_PIN 5                             /* Toggled by software to select between live video and spectrum analyser. */

/* LEDs. */
#define LED_RX_1 6                                      /* RX1 status LED. */
#define LED_RX_2 7                                      /* RX2 status LED. */
#define LED_025_P 8                                     /* RSSI 0%-25% status LED. */
#define LED_050_P 9                                     /* RSSI 26%-50% status LED. */
#define LED_075_P 10                                    /* RSSI 51%-75% status LED. */
#define LED_100_P 11                                    /* RSSI 76%-100% status LED. */
#define LED_DIVERSITY 12                                /* Diversity active indicator LED. */
#define LED_HEARTBEAT 13                                /* Heartbeat, indicates loop speed, for initial debug. */

/* Delays. */
#define ADC_WARMUP_DELAY 1                              /* Time to let ADCs settle. Default 1. */
#define BUTTON_DEBOUNCE_MILLIS 250                      /* Push button debounce value. Default 250. */
#define DIVERSITY_INTERVAL_MILLIS 2000                  /* Minimum allowable time before video pin toggles. Default 500. */
#define CALIB_STAB_CYCLES 1000                          /* Calibration stabilization cycles. Give pleanty of cycles for averages to become stable figures. Default 1000. */
#define CALIB_TIMEOUT_MILLIS 30000                      /* Give up calibrating after X seconds. Default 30000. */
#define ONE_TIME_WARMUP_DELAY 5000                      /* A one time delay to allow voltages to stabilize on the TX and RC model during Auto Calibration. Default 5000. */
#define CALIB_RETRY_INITIAL_DELAY 10000                 /* Wait period before starting calibration after first attempt fails. Default 5000. */

/* Info. */
#define AUTHOR "MIKE DALTON"                            /* Version info. */
#define INFO_MAJOR_VERSION "1"                          /* Version info. */
#define INFO_MINOR_VERSION "9"                          /* Version info. */
#define COMPILE_TIME __TIME__                           /* Version info. */
#define COMPILE_DATE __DATE__                           /* Version info. */
#define SOURCE_FILE __FILE__                            /* Version info. */
#define COMPILER __VERSION__                            /* Version info. */

/* RSSI. */
#define MAX_AVERAGE_READINGS 10                         /* Smoothing value for RSSI averages. Default 10. */
#define RSSI_HYSTERESIS 1                               /* This hysteresis value is a % of the calculated RSSI value. Default 1. */

                                                        /* RSSI voltage range is between 0.5v and 1.1v for most rx5808 modules. */
unsigned int RSSI1Min = 512;                            /* Default 512. */
unsigned int RSSI1Max = 1023;                           /* 1024 = 1.1V when using internal voltage reference. Default 1024. */
unsigned int RSSI2Min = 512;                            /* Default 512. */
unsigned int RSSI2Max = 1023;                           /* 1024 = 1.1V when using internal voltage reference. Default 1024. */
int RSSI1P = 0;                                         /* Mapped RSSI value as a percentage - taken from min and max values. */
int RSSI2P = 0;                                         /* Mapped RSSI value as a percentage - taken from min and max values. */
unsigned int RSSI1Readings[MAX_AVERAGE_READINGS];       /* The readings from the analogue input. */
unsigned int RSSI1ReadIndex = 0;                        /* The index of the current reading. */
unsigned int RSSI1Total = 0;                            /* The running total. */
unsigned int RSSI1Average = 0;                          /* The average RSSI. */
unsigned int RSSI1InputPinValue = 0;                    /* ADC reading. */
unsigned int RSSI2Readings[MAX_AVERAGE_READINGS];       /* The readings from the analogue input. */
unsigned int RSSI2ReadIndex = 0;                        /* The index of the current reading. */
unsigned int RSSI2Total = 0;                            /* The running total. */
unsigned int RSSI2Average = 0;                          /* The average RSSI. */
unsigned int RSSI2InputPinValue = 0;                    /* ADC reading. */

/* Voltage. */
float ADC_MAX = 1024.0;                                 /* Max ADC value. */
float RSSIARef = 1.1;                                   /* Voltage of selected analogue reference. (see below, setup) */
float RSSI1Volts = 0;                                   /* Calculated voltage on RSSI pin. */
float RSSI2Volts = 0;                                   /* Calculated voltage on RSSI pin. */

/* Buttons. */
int VideoControlPinState = LOW;                         /* The current state of the output pin. */
int VideoSwitchCounter = 1;                             /* Counter for mode selection. */
int VideoSwitchReading;                                 /* The current reading from the input pin. */
int VideoSwitchPrevious = HIGH;                         /* The previous reading from the input pin. */
long VideoSwitchTime = 0;                               /* The last time the output pin was toggled. */

int RxControlPinState = LOW;                            /* The current state of the output pin. */
int ModeSwitchCounter = 1;                              /* Counter for mode selection - 3 modes. (mode 1= rx1, mode 2= rx2, mode 3= diversity) */
int ModeSwitchReading;                                  /* The current reading from the input pin. */
long ModeSwitchTime = 0;                                /* The last time the output pin was toggled. */

/* Flags. */
boolean DebugMode = false;                              /* State of debug mode. */
boolean AutoRSSIMode = false;                           /* State of auto RSSI mode. */
boolean MinRSSICalibrationFlag = false;                 /* False = fail, true = pass. */
boolean MaxRSSICalibrationFlag = false;                 /* False = fail, true = pass. */
boolean MinCalibRunOnce = false;
boolean MaxCalibRunOnce = false;
boolean RSSICalibrationCompleteFlag = false;            /* Set after both RSSI figures have been updated with on the fly values. */
boolean OneTimeWarmupDelayFlag = true;                  /* True until one cycle of auto calibration of high RSSI has been attempted. */
boolean CalibRetryDelayFlag = false;                    /* Initiate a delay before restarting Recalibration. */
boolean RecalibrateButtonCheck = true;                  /* Check for a button press to initiate a recalibration cycle. */
boolean Recalibrate = false;                            /* Enter Recalibration function. */
boolean StartMinCal = false;
boolean MincalibrateButtonCheck = true;
boolean StartMaxCal = false;
boolean MaxcalibrateButtonCheck = true;


/* Diversity. */
unsigned long CounterPreviousDiversitySwitchTime = 0;   /* Counter for DIVERSITY_INTERVAL_MILLIS. */
unsigned int AutoRSSICalLowLevel = 40;                  /* % RSSI FOR AUTO CAL. Default 30. */
unsigned int AutoRSSICalHighLevel = 60;                 /* % RSSI FOR AUTO CAL. Default 70. */
unsigned int RSSI1TempMin = 0;                          /* Used during auto calibration. */
unsigned int RSSI1TempMax = 0;                          /* Used during auto calibration. */
unsigned int RSSI2TempMin = 0;                          /* Used during auto calibration. */
unsigned int RSSI2TempMax = 0;                          /* Used during auto calibration. */

/* Calibration. */
unsigned int CalibrationCyclesCounter = 0;              /* Counter to make sure average RSSI readings have stabilized during calibration. */
unsigned long CalibrationTimeoutCounter = 0;            /* Allow us to bail out of auto calibration if readings aren't within specification. */
unsigned long RecalibrationOffsetCounter = 0;


void setup() {

    pinMode(RSSI1_ADC_PIN, INPUT);
    pinMode(RSSI2_ADC_PIN, INPUT);

    pinMode(MODE_SWITCH, INPUT_PULLUP);                 /* Hold during power up to enter debug mode. */
    pinMode(VIDEO_SWITCH, INPUT_PULLUP);                /* Hold during power up to enter auto RSSI calibration mode mode. */
                                                        /* Both buttons can be held simultaneously. */
    pinMode(RX_CONTROL_PIN, OUTPUT);
    pinMode(VIDEO_CONTROL_PIN, OUTPUT);

    pinMode(LED_RX_1, OUTPUT);
    pinMode(LED_RX_2, OUTPUT);

    pinMode(LED_025_P, OUTPUT);
    pinMode(LED_050_P, OUTPUT);
    pinMode(LED_075_P, OUTPUT);
    pinMode(LED_100_P, OUTPUT);

    pinMode(LED_DIVERSITY, OUTPUT);
    pinMode(LED_HEARTBEAT, OUTPUT);


    analogReference(INTERNAL);   /* Use internal 1V1 reference. */
    //analogReference(DEFAULT);  /* Use supply voltage as reference. (3V3 / 5V) */
    //analogReference(EXTERNAL); /* Use a voltage reference IC of your choice on the AREF pin. (EXAMPLE = FARNELL.COM:175-5111 / LM4040B20IDBZT) */
    /* The exact figure must be entered into "RSSIARef" in the Voltage section above to calculate correct debug info. */

    /* Warm up ADCs */
    RSSI1InputPinValue = analogRead(RSSI1_ADC_PIN);
    delay(ADC_WARMUP_DELAY);
    RSSI1InputPinValue = analogRead(RSSI1_ADC_PIN);
    delay(ADC_WARMUP_DELAY);
    RSSI1InputPinValue = analogRead(RSSI1_ADC_PIN);
    delay(ADC_WARMUP_DELAY);
    RSSI2InputPinValue = analogRead(RSSI2_ADC_PIN);
    delay(ADC_WARMUP_DELAY);
    RSSI2InputPinValue = analogRead(RSSI2_ADC_PIN);
    delay(ADC_WARMUP_DELAY);
    RSSI2InputPinValue = analogRead(RSSI2_ADC_PIN);
    delay(ADC_WARMUP_DELAY);

    /* RSSI averaging setup */
    for(int ReadingCurrent = 0; ReadingCurrent < MAX_AVERAGE_READINGS; ReadingCurrent++)
    {
        RSSI1Readings[ReadingCurrent] = 0;
        RSSI2Readings[ReadingCurrent] = 0;
    }

    digitalWrite(RX_CONTROL_PIN, LOW);        /* Default RX module on start-up. */
    digitalWrite(VIDEO_CONTROL_PIN, LOW);     /* Default screen (live video / spectrum analyser) on start-up. */

    digitalWrite(LED_025_P, HIGH);            /* flash some LEDs. */
    delay(150);
    digitalWrite(LED_050_P, HIGH);
    delay(150);
    digitalWrite(LED_075_P, HIGH);
    delay(150);
    digitalWrite(LED_100_P, HIGH);
    delay(150);
    digitalWrite(LED_RX_1, HIGH);
    digitalWrite(LED_RX_2, HIGH);
    digitalWrite(LED_DIVERSITY, HIGH);
    delay(100);
    digitalWrite(LED_025_P, LOW);
    digitalWrite(LED_050_P, LOW);
    digitalWrite(LED_075_P, LOW);
    digitalWrite(LED_100_P, LOW);
    digitalWrite(LED_RX_1, LOW);
    digitalWrite(LED_RX_2, LOW);
    digitalWrite(LED_DIVERSITY, LOW);


    if (digitalRead(MODE_SWITCH) == 0)         /* Holding Mode switch enters DebugMode. */
    {
        DebugMode = true;
    }

    else
    {
        DebugMode = false;
    }

    if (digitalRead(VIDEO_SWITCH) == 0)        /* Holding Video switch enters RSSI calibration mode. */
    {
        AutoRSSIMode = true;
    }

    else
    {
        AutoRSSIMode = false;
    }


    if(DebugMode == true)
    {
        Serial.begin(19200);                    /* Start serial terminal if in debug mode. */
        Serial.println(" ");
        Serial.println(" ");
        Serial.println(" ");
        Serial.println("Dual 5.8ghz Video Receiver Diversity Controller with RX5808-PRO functionality.");
        Serial.println(" ");
        Serial.print(AUTHOR);
        Serial.print(", V");
        Serial.print(INFO_MAJOR_VERSION);
        Serial.print(".");
        Serial.println(INFO_MINOR_VERSION);

        Serial.print(COMPILE_DATE);
        Serial.print(" - ");
        Serial.println(COMPILE_TIME);
        Serial.println(SOURCE_FILE);
        Serial.print("Compiled with = ");
        Serial.println(COMPILER);

        Serial.println(" ");
        Serial.println("!!!DEBUG MODE!!!");
        Serial.println(" ");
        delay(200);

        if(AutoRSSIMode == true)
        {
            Serial.println("!!!AUTO RSSI CALIBRATION MODE!!!");
            Serial.println(" ");
            delay(200);
        }

        else
        {
        /* Do Nothing */
        }
    }
}



/*******************************************************************************
loop

*******************************************************************************/
void loop()
{
    digitalWrite(LED_HEARTBEAT, HIGH);



    /*******************************************************************************
    VIDEO TOGGLING SECTION

    Selects video source on selected video receiver. Each receiver has two video
    outputs, live video and spectrum analyser.
    *******************************************************************************/



    VideoSwitchReading = digitalRead(VIDEO_SWITCH);  /* Read the value of the video switch and save */

if((VideoSwitchReading == LOW) && ((millis() - VideoSwitchTime) > BUTTON_DEBOUNCE_MILLIS))
    {
        VideoSwitchCounter++;

        if(VideoSwitchCounter >= 3) /* Reset count if over max mode number */
        {
            VideoSwitchCounter = 1;
        }

        VideoSwitchTime = millis();
    }



    /******************************************************************************
    MODE TOGGLING SECTION

    EACH BUTTON PRESS SHOULD MOVE BETWEEN MODES 1-2-3-1-2-3.....
    ******************************************************************************/



    ModeSwitchReading = digitalRead(MODE_SWITCH); /* Read the value of the mode switch and save */

    if((ModeSwitchReading == LOW) && ((millis() - ModeSwitchTime) > BUTTON_DEBOUNCE_MILLIS))
    {
        ModeSwitchCounter++;

        if(ModeSwitchCounter >= 4) /* Reset count if over max mode number */
        {
            ModeSwitchCounter = 1;
        }

        ModeSwitchTime = millis();
    }



  /******************************************************************************
    RSSI CALCULATIONS

    RSSI figures need averaging, turning into a percentage and clipping.
    ******************************************************************************/



    RSSI1InputPinValue = analogRead(RSSI1_ADC_PIN);
    RSSI1Total = RSSI1Total - RSSI1Readings[RSSI1ReadIndex];
    RSSI1Readings[RSSI1ReadIndex] = analogRead(RSSI1_ADC_PIN);
    RSSI1Total = RSSI1Total + RSSI1Readings[RSSI1ReadIndex];
    RSSI1ReadIndex = RSSI1ReadIndex + 1;

    if(RSSI1ReadIndex >= MAX_AVERAGE_READINGS)
    {
        RSSI1ReadIndex = 0;
    }

    RSSI1Average = RSSI1Total / MAX_AVERAGE_READINGS;

    RSSI2InputPinValue = analogRead(RSSI2_ADC_PIN);
    RSSI2Total = RSSI2Total - RSSI2Readings[RSSI2ReadIndex];
    RSSI2Readings[RSSI2ReadIndex] = analogRead(RSSI2_ADC_PIN);
    RSSI2Total = RSSI2Total + RSSI2Readings[RSSI2ReadIndex];
    RSSI2ReadIndex = RSSI2ReadIndex + 1;

    if(RSSI2ReadIndex >= MAX_AVERAGE_READINGS)
    {
        RSSI2ReadIndex = 0;
    }

    RSSI2Average = RSSI2Total / MAX_AVERAGE_READINGS;

    RSSI1P = map(RSSI1Average, RSSI1Min, RSSI1Max, 0, 100) + 1;
    RSSI2P = map(RSSI2Average, RSSI2Min, RSSI2Max, 0, 100) + 1;

    /* Clip erroneous values to within 0%-100% range */
    if(AutoRSSIMode == true)    /* clip Min and Max RSSI values so as not to calculate negative numbers as % for pre calibration limit check. WIP. */
    {
        if(RSSI1P < 0)
        {
            RSSI1P = 0;
        }

        else if(RSSI1P > 100)
        {
            RSSI1P = 100;
        }

        else
        {
            /* Do Nothing */
        }

         if(RSSI2P < 0)
        {
            RSSI2P = 0;
        }

        else if(RSSI2P > 100)
        {
            RSSI2P = 100;
        }

        else
        {
            /* Do Nothing */
        }
    }

    /* Calculate voltages of RSSI pins. */
    RSSI1Volts = (RSSI1InputPinValue / ADC_MAX) * RSSIARef;
    RSSI2Volts = (RSSI2InputPinValue / ADC_MAX) * RSSIARef;



    /******************************************************************************
    AUTOMATIC RSSI CALIBRATION

    If the flag was set during setup, we will jump to the calibration loop to
    attempt auto calibration.
    We will either be successful or a "time-out" will occur.
    ******************************************************************************/



    while(AutoRSSIMode == true) /* Video button was held during power up (pulled low) */
    {
        Calibrate();            /* Jump to calibration loop. */
    }



    /******************************************************************************
    LED CONTROL SECTION

    LEDs must be driven appropriately to display active video receiver
    and RSSI levels.

    Mode 1 selects video receiver one, Mode 2 selects video receiver 2 and mode 3
    selects the receiver with the highest RSSI value. The RSSI value is read
    with each pass through the loop and the receiver with the highest value is
    selected.
    ******************************************************************************/



    digitalWrite(LED_RX_1, (RxControlPinState == LOW));       /* When "RxControlPinState" is low the rx1 led will illuminate. Place this led next to RX1 antenna */
    digitalWrite(LED_RX_2, (!(RxControlPinState == LOW)));    /* When "RxControlPinState" is high the rx2 led will illuminate. Place this led next to RX2 antenna */

    /* Smoothly display RSSI on four LEDs */

    if(RxControlPinState == LOW)
    {
        digitalWrite(LED_025_P, (RSSI1P > 0));
        digitalWrite(LED_050_P, (RSSI1P > 25));
        digitalWrite(LED_075_P, (RSSI1P > 50));
        digitalWrite(LED_100_P, (RSSI1P > 75));
    }

    else
    {
        digitalWrite(LED_025_P, (RSSI2P > 0));
        digitalWrite(LED_050_P, (RSSI2P > 25));
        digitalWrite(LED_075_P, (RSSI2P > 50));
        digitalWrite(LED_100_P, (RSSI2P > 75));
    }

    digitalWrite(LED_HEARTBEAT, LOW); // TODO MD 20150317 Clean this up and make it work better.



    /******************************************************************************
    MODE SPECIFIC ACTIONS

    Physical pins are toggled in accordance with mode selection.
    In diversity mode a timer is present to stop "thrashing" of the receiver
    selection pin thus reducing screen flicker.

    Delay is calculated with "DIVERSITY_INTERVAL_MILLIS".
    Hysteresis is applied using "RSSI_HYSTERESIS".
    ******************************************************************************/


    /* Video button. Switches between live video and spectrum analyser output.*/
    if(VideoSwitchCounter == 1)
    {
        VideoControlPinState = LOW;
    }

    else if(VideoSwitchCounter == 2)
    {
        VideoControlPinState = HIGH;
    }

    digitalWrite(VIDEO_CONTROL_PIN, VideoControlPinState);

    /* Mode button. */
    unsigned long CounterCurrentDiversitySwitchTime = millis();
    unsigned long elapsed = CounterCurrentDiversitySwitchTime - CounterPreviousDiversitySwitchTime;

    digitalWrite(LED_DIVERSITY, (ModeSwitchCounter >= 3));

    if(ModeSwitchCounter == 1)
    {
        RxControlPinState = LOW;
    }

    else if(ModeSwitchCounter == 2)
    {
        RxControlPinState = HIGH;
    }

    else if(ModeSwitchCounter == 3)
    {
        //digitalWrite(LED_DIVERSITY, HIGH); /* Display diversity mode */
        // lets see if "digitalWrite(LED_DIVERSITY, (ModeSwitchCounter >= 3));" will do the trick. MD

        if(elapsed > DIVERSITY_INTERVAL_MILLIS)
        {

            if(RSSI1P + RSSI_HYSTERESIS > RSSI2P)
            {
                RxControlPinState = LOW;
            }

            else if(RSSI2P + RSSI_HYSTERESIS > RSSI1P)
            {
                RxControlPinState = HIGH;
            }

            else
            {
             /* Do Nothing */
            }
            CounterPreviousDiversitySwitchTime = CounterCurrentDiversitySwitchTime;
        }
    }

    else
    {
    /* Do Nothing */
    }

    digitalWrite(RX_CONTROL_PIN, RxControlPinState);



    /******************************************************************************
    SERIAL DEBUG MESSAGES

    Display values in serial console when in serial debug mode.
    Comment / uncomment as necessary.
    ******************************************************************************/



    if(DebugMode == true)
    {

    /* RSSI 1. */

    //    Serial.print("  RSSI1 =");
    //    Serial.print(RSSI1InputPinValue);

        Serial.print("  RSSI1_AVERAGE =");
        Serial.print(RSSI1Average);

        Serial.print("  RSSI1% =");
        Serial.print(RSSI1P);

    //    Serial.print("  RSSI1Volts =");
    //    Serial.print(RSSI1Volts);

    //    Serial.print("  RSSI1 MIN/MAX =");
    //    Serial.print(RSSI1Min);
    //    Serial.print("/");
    //    Serial.print(RSSI1Max);


    /* RSSI 2. */

    //    Serial.print("  RSSI2 =");
    //    Serial.print(RSSI2InputPinValue);

        Serial.print("  RSSI2_AVERAGE =");
        Serial.print(RSSI2Average);

        Serial.print("  RSSI2% =");
        Serial.print(RSSI2P);

    //    Serial.print("  RSSI2Volts =");
    //    Serial.print(RSSI2Volts);

    //    Serial.print("  RSSI2 MIN/MAX =");
    //    Serial.print(RSSI2Min);
    //    Serial.print("/");
    //    Serial.print(RSSI2Max);


    /* Switches. */

        Serial.print("  VIDEO =");                  /* Live video / Spectrum analyser switch */
        Serial.print(VideoControlPinState);

        Serial.print("  MODE =");                   /* Current mode (3 IS DIVERSITY) */
        Serial.print(ModeSwitchCounter);

        Serial.print("  RX =");                     /* state of video RX selector pin. */
        Serial.println(RxControlPinState);

        Serial.print("  Elapsed ");               /* Time since RX modules were toggled. */
        Serial.print(elapsed);

        Serial.print("  AutoCal ");
        Serial.print(RSSICalibrationCompleteFlag);

    }
}



/******************************************************************************
Calibrate - AUTOMATIC RSSI CALIBRATION LOOP

Attempt to auto calibrate RSSI levels by reading current RSSI value for each
video receiver, making sure that they are below 30% (TX off) before starting.
Let the average counter run for 1000 cycles and then set this value for the
MIN RSSI value for each video receiver. Set the first calibration flag.
let the average counter run for 1000 cycles and then set this value for the
MAX RSSI for each video receiver. Set the second calibration flag.
Check that both the MIN and MAX flags have been set, set the calibration done
flag and apply temp settings as RSSI MIN and MAX values, flash appropriate
LED and set the calibration complete flag. Show serial messages as appropriate.

NOTE:
Video channel must be selected on both RX units prior to attempting Auto
Calibration.

Turn on diversity receiver in Auto Calibration Mode by holding the "Video"
button during power up, amber LED will illuminate whilst Min RSSI values
are being calculated, turn on transmitter, a second amber led will illuminate
as Max RSSI is being calculated.

If Automatic RSSI calibration is successful the green LED will illuminate.
The red LED indicates failure to calibrate.

In the event that calibration is unsuccessful the default values will be
applied and the unit will return to normal operation.

If calibration is successful, calculated values will remain active until unit
is turned off.

Auto Calibration RSSI values ARE NOT saved to EEPROM!
Directional antennas MUST be pointed at TX module during calibration!
Make sure NO ONE else is broadcasting on the same channel prior to Calibration!
YOU are solely responsible for the operation of YOUR equipment!
******************************************************************************/

// Maybe some button presses should be included to initiate the calibration routine rather than relying on time-outs and auto detection.
// flash some leds to indicate cal mode, press mode to start low cal, illuminate red or green - depending on outcome... if its a fail, hold video to restart.
// if  its green, press video to continue to high cal, if its a fail, hold video to restart else save figures and exit.

void Calibrate(void)
{
    while(MinRSSICalibrationFlag == false)   /* Jump to min calibration until flag changes, at this point we have temp figures to apply later. */
    {
        CalibrateMin();         /* Jump to Min calibration loop. */
    }

    while(MaxRSSICalibrationFlag == false)   /* Jump to max calibration until flag changes, at this point we have temp figures to apply later. */
    {
        CalibrateMax();         /* Jump to Max calibration loop. */
    }

    if(MinRSSICalibrationFlag == true && MaxRSSICalibrationFlag == true)     /* Calibration successful. */
    {
        RSSI1Min = RSSI1TempMin; /* Save temp value as new min RSSI value */
        RSSI2Min = RSSI2TempMin; /* Save temp value as new min RSSI value */
        RSSI1Max = RSSI1TempMax; /* Save temp value as new max RSSI value */
        RSSI2Max = RSSI2TempMax; /* Save temp value as new max RSSI value */

        digitalWrite(LED_100_P, HIGH);         /* Green light, good job!! */
        RSSICalibrationCompleteFlag = true;    /* Flag set, Calibration successful, we wont try calibration again! */
        AutoRSSIMode = false;                  /* Flag set, we wont be coming back into calibration loop again! */

        if(DebugMode == true)
        {
            Serial.println("  ");
            Serial.println("AUTO RSSI CALIBRATION COMPLETE, NEW SETTINGS APPLIED!");
            Serial.print("RSSI1 MIN / MAX = ");
            Serial.print(RSSI1Min);
            Serial.print(" / ");
            Serial.println(RSSI1Max);
            Serial.print("RSSI2 MIN / MAX = ");
            Serial.print(RSSI2Min);
            Serial.print(" / ");
            Serial.println(RSSI2Max);
            Serial.println("Restarting using calculated values....");
        }
        delay(2500);

    }

}

void CalibrateMin (void)
{

    /* do nothing until a button press is sensed, find the average, save it as a temp value, */



        if(MinCalibrateButtonCheck == true)    /* Only check for button press once. */
        {
            if(digitalRead(MODE_SWITCH) == 0)
            {
                StartMinCal = true;
                MinCalibrateButtonCheck = false;       /* we must successfully jump to recalibration loop if we ever want this to toggle true again. sanity check. */

                if(DebugMode == true)
                    {
                        Serial.println("Attempting Min calibration....");
                    }
            }
        }


        if(MinRSSICalibrationFlag == false && StartMinCal == false)
        {
            /* We are waiting for a button press to start calibration. */
            /* Flash an LED and spit out serial debug whilst waiting. */
            /* We will be stuck here until a button is pressed or a time-out occurs. */
        }


    if(RSSI1P >= AutoRSSICalLowLevel && RSSI2P >= AutoRSSICalLowLevel && StartMinCal == true)   /* Error condition, RSSI high. */
    {
    /* We have an error condition, the RSSI is too high to start calibration. */
    }

    if(RSSI1P <= AutoRSSICalLowLevel && RSSI2P <= AutoRSSICalLowLevel && StartMinCal == true)    /* Calibration. */
    /* Make sure we're under 40% (if the AREF was set-up properly, this is a reasonable figure) */
    /* We are assuming that transmitters are turned off and receivers are tuned correctly at this point. */
    /* Fail Auto Calibrate if both receivers ARE NOT showing values of less than 40% RSSI. */
    {
        digitalWrite(LED_050_P, HIGH);     /* Indicate that we are in LOW calibration mode. */
        MinCalibRunOnce = true;


        CalibrationCyclesCounter = CalibrationCyclesCounter + 1;   /* Start counting cycles through the loop. */

        if(DebugMode == true)
        {
            Serial.print("CALCULATE MIN RSSI... ");
            Serial.print(CalibrationCyclesCounter);
            Serial.print("/");
            Serial.println(CALIB_STAB_CYCLES);
        }

        if(CalibrationCyclesCounter >= CALIB_STAB_CYCLES) /* Ignore this line until X cycles have been reached, then set the RSSI readings as a temp value */
        {
            RSSI1TempMin = RSSI1Average;
            RSSI2TempMin = RSSI2Average;
            MinRSSICalibrationFlag = true; /* Set the Min calibration flag to "done" (true) so that the above statements get ignored next time through the loop */

            if(DebugMode == true)
            {
                Serial.print("Saving temp figures...");
            }

            MinRSSICalibrationFlag = true; /* Set the Min calibration flag to "done" (true) so that the above statements get ignored next time through the loop */

            if(DebugMode == true)
            {
                Serial.println("MIN RSSI CALIBRATION COMPLETE!");
                Serial.print("RSSI1 = ");
                Serial.println(RSSI1TempMin);
                Serial.print("RSSI2 = ");
                Serial.println(RSSI2TempMin);
                delay(1000);
            }

            CalibrationCyclesCounter = 0;    /* Reset the counter as we are going to use it again for the high RSSI calibration. */
        }


    }


}

void CalibrateMax (void)
{

    /* do nothing until a button press is sensed, find the average, save it as a temp value, */



        if(MaxCalibrateButtonCheck == true)    /* Only check for button press once. */
        {
            if(digitalRead(MODE_SWITCH) == 0)
            {
                StartMaxCal = true;
                MaxCalibrateButtonCheck = false;       /* we must successfully jump to recalibration loop if we ever want this to toggle true again. sanity check. */

                if(DebugMode == true)
                    {
                        Serial.println("Attempting Max calibration....");
                    }
            }
        }


        if(MaxRSSICalibrationFlag == false && StartMaxCal == false)
        {
            /* We are waiting for a button press to start calibration. */
            /* Flash an LED and spit out serial debug whilst waiting. */
            /* We will be stuck here until a button is pressed or a time-out occurs. */
        }


    if(RSSI1P <= AutoRSSICalHighLevel && RSSI2P <= AutoRSSICalHighLevel && StartMaxCal == true) /* Error condition, RSSI low. */
    {
    /* We have an error condition, the RSSI is too low to start calibration. */
    }

    if(RSSI1P >= AutoRSSICalHighLevel && RSSI2P >= AutoRSSICalHighLevel && StartMaxCal == true) /* Make sure we're over 70% (TX ON and tuned) for both RX units. */
        {
            digitalWrite(LED_075_P, HIGH);     /* Indicate that we are in HIGH calibration mode. */
            MaxCalibRunOnce = true;


            CalibrationCyclesCounter = CalibrationCyclesCounter + 1;

            if(DebugMode == true)
            {
                Serial.print("CALCULATE MAX RSSI...   ");
                Serial.print(CalibrationCyclesCounter);
                Serial.print("/");
                Serial.println(CALIB_STAB_CYCLES);
            }

            if(CalibrationCyclesCounter >= CALIB_STAB_CYCLES)    /* Ignore this line until X cycles have been reached, then set the RSSI readings as a temp value */
            {
                RSSI1TempMax = RSSI1Average;
                RSSI2TempMax = RSSI2Average;
                MaxRSSICalibrationFlag = true; /* Set the Max calibration flag to "done" (true) so that the above get ignored next time through the loop */

                if(DebugMode == true)
                {
                    Serial.print("Saving temp figures...");
                }



                if(DebugMode == true)
                {
                    Serial.println("MAX RSSI CALIBRATION COMPLETE!");
                    Serial.print("RSSI1 = ");
                    Serial.println(RSSI1TempMax);
                    Serial.print("RSSI2 = ");
                    Serial.println(RSSI2TempMax);
                    delay(1000);
                }

                CalibrationCyclesCounter = 0;
            }
        }
}



/******************************************************************************
 ResetCalibration - Reinitialise flags to enable another calibration attempt.

Modify flags and timers.
Increased delay before starting Low calibration routine to enable time to
power cycle TX gear.
Flash some LEDs to indicate Recalibration has begun.
******************************************************************************/


void ResetCalibration(void)    /* This works fine. (with old calibration routine.) */
{
    // DROP INTO THIS LOOP TO RESET FLAGS AND TIMERS.

    digitalWrite(LED_025_P, LOW);
    digitalWrite(LED_050_P, LOW);
    digitalWrite(LED_075_P, LOW);
    digitalWrite(LED_100_P, LOW);

    if(DebugMode == true)
    {
        Serial.print("Resetting flags....");
    }

    /* Reset flags to default values, enable another calibration cycle. */
    AutoRSSIMode = true;
    MinRSSICalibrationFlag = false;
    MaxRSSICalibrationFlag = false;
    MinCalibRunOnce = false;
    MaxCalibRunOnce =false;
    RSSICalibrationCompleteFlag = false;
    //OneTimeWarmupDelayFlag = true;

    StartMinCal = false;
    MinCalibrateButtonCheck = true;

    StartMaxCal = false;
    MaxCalibrateButtonCheck = true;

    RecalibrateButtonCheck = true;  /* Allow us to listen out for another recalibration attempt. */
    //CalibRetryDelayFlag = true;     /* add X second delay at the start of the next calibration attempt. */
    Recalibrate = false;    /* Next time through the loop, we wont be coming back here unless button is pressed again. */

    if(DebugMode == true)
    {
        Serial.println("Done!");
    }

    //RecalibrationOffsetCounter = (CalibrationTimeoutCounter + CALIB_RETRY_INITIAL_DELAY);  /* add X seconds of time (since power up) to the timeout counter. */

    if(DebugMode == true)
    {
    //  Serial.println("Time-out counter has been modified...");
        Serial.println("Restarting Calibration...");
    }

    digitalWrite(LED_025_P, HIGH);            /* flash some LEDs. */
    digitalWrite(LED_050_P, HIGH);
    digitalWrite(LED_075_P, HIGH);
    digitalWrite(LED_100_P, HIGH);
    delay(100);
    digitalWrite(LED_025_P, LOW);
    digitalWrite(LED_050_P, LOW);
    digitalWrite(LED_075_P, LOW);
    digitalWrite(LED_100_P, LOW);
    delay(100);
    digitalWrite(LED_025_P, HIGH);            /* flash some LEDs. */
    digitalWrite(LED_050_P, HIGH);
    digitalWrite(LED_075_P, HIGH);
    digitalWrite(LED_100_P, HIGH);
    delay(100);
    digitalWrite(LED_025_P, LOW);
    digitalWrite(LED_050_P, LOW);
    digitalWrite(LED_075_P, LOW);
    digitalWrite(LED_100_P, LOW);
    delay(100);
    digitalWrite(LED_025_P, HIGH);            /* flash some LEDs. */
    digitalWrite(LED_050_P, HIGH);
    digitalWrite(LED_075_P, HIGH);
    digitalWrite(LED_100_P, HIGH);
    delay(100);
    digitalWrite(LED_025_P, LOW);
    digitalWrite(LED_050_P, LOW);
    digitalWrite(LED_075_P, LOW);
    digitalWrite(LED_100_P, LOW);
    delay(100);
    digitalWrite(LED_025_P, HIGH);            /* flash some LEDs. */
    digitalWrite(LED_050_P, HIGH);
    digitalWrite(LED_075_P, HIGH);
    digitalWrite(LED_100_P, HIGH);
    delay(100);
    digitalWrite(LED_025_P, LOW);
    digitalWrite(LED_050_P, LOW);
    digitalWrite(LED_075_P, LOW);
    digitalWrite(LED_100_P, LOW);
    delay(100);


}    /* Back to beginning of main loop, followed by calibration. */







