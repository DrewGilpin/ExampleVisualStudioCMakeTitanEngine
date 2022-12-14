/******************************************************************************
 * Copyright (c) Grzegorz Slazinski. All Rights Reserved.                     *
 * Titan Engine (https://esenthel.com) header file.                           *
/******************************************************************************

   Use 'Joypads' container to access Joypads input.

/******************************************************************************/
enum JOYPAD_BUTTON : Byte // button indexes as defined for XInput/Xbox/NintendoSwitch controllers
{
   JB_A    , // A
   JB_B    , // B
   JB_X    , // X
   JB_Y    , // Y
   JB_L1   , // Left  Bumper
   JB_R1   , // Right Bumper
   JB_L2   , // Left  Trigger
   JB_R2   , // Right Trigger
   JB_L3   , // Left  Stick
   JB_R3   , // Right Stick
   JB_BACK , // Back
   JB_START, // Start

   JB_LSL, // Nintendo Switch Left  SL
   JB_LSR, // Nintendo Switch Left  SR
   JB_RSL, // Nintendo Switch Right SL
   JB_RSR, // Nintendo Switch Right SR

   JB_HOME, // Home

   // alternative names
   JB_LB=JB_L1,
   JB_RB=JB_R1,
   JB_LT=JB_L2,
   JB_RT=JB_R2,
   JB_LS=JB_L3,
   JB_RS=JB_R3,

   JB_SELECT=JB_BACK , // Sony Playstation
   JB_MINUS =JB_BACK , // Nintendo Switch
   JB_PLUS  =JB_START, // Nintendo Switch

   JB_MINI_S1=JB_LSL, // Nintendo Switch Mini Side Button 1
   JB_MINI_S2=JB_LSR, // Nintendo Switch Mini Side Button 2

   JB_L=JB_X, // button located at the Left  side
   JB_R=JB_B, // button located at the Right side
   JB_U=JB_Y, // button located at the Up    side
   JB_D=JB_A, // button located at the Down  side

   JB_SQUARE  =JB_L, // button located at the Left  side
   JB_CIRCLE  =JB_R, // button located at the Right side
   JB_TRIANGLE=JB_U, // button located at the Up    side
   JB_CROSS   =JB_D, // button located at the Down  side

   // these are valid only for 'Inputs'
   JB_DPAD_LEFT =32,
   JB_DPAD_RIGHT=33,
   JB_DPAD_DOWN =34,
   JB_DPAD_UP   =35,
};
/******************************************************************************/
struct Vibration
{
   struct Motor
   {
      Flt intensity, // intensity, 0..1
          frequency; // frequency, Hertz
   };
   Motor motor[2];
};
/******************************************************************************/
struct Joypad // Joypad Input
{
   struct Color2
   {
      Color main, sub;

      Color2& zero()  {       main.zero();   sub.zero(); return T;}
      Bool    any ()C {return main.any () || sub.any ();}
   };
   struct Sensor
   {
      Vec    accel, // accelerometer
             gyro ; // gyroscope
      Orient orn  ; // orientation

      Sensor& reset() {accel.zero(); gyro.zero(); orn.identity(); return T;}
   };

   VecSB2 diri      , //        direction (integer version)
          diri_r    , //        direction (integer version) repeatable, will get triggered repeatedly as long as        direction is pressed
          diri_ar[2]; // analog direction (integer version) repeatable, will get triggered repeatedly as long as analog direction is pressed, [0]=left, [1]=right
   Vec2   dir       , //        direction
          dir_a  [2]; // analog direction, [0]=left, [1]=right
   Flt    trigger[2]; // trigger         , [0]=left, [1]=right

   BS_FLAG state(Int b)C {return InRange(b, _button) ?          _button[b]  : BS_NONE;} // get button 'b' state
   Bool    b    (Int b)C {return InRange(b, _button) ? ButtonOn(_button[b]) :   false;} // if  button 'b' is on
   Bool    bp   (Int b)C {return InRange(b, _button) ? ButtonPd(_button[b]) :   false;} // if  button 'b' pushed   in this frame
   Bool    br   (Int b)C {return InRange(b, _button) ? ButtonRs(_button[b]) :   false;} // if  button 'b' released in this frame
   Bool    bd   (Int b)C {return InRange(b, _button) ? ButtonDb(_button[b]) :   false;} // if  button 'b' double clicked

   void eat(Int b); // eat 'b' button from this frame so it will not be processed by the remaining codes in frame, this disables all BS_FLAG states (BS_PUSHED, BS_RELEASED, etc.) except BS_ON

   Bool supportsVibrations()C; // if supports vibrations
   Bool supportsSensors   ()C; // if supports sensors, available only if 'JoypadSensors' was enabled

           UInt           id  (     )C {return _id  ;} // get unique ID of this Joypad
          C Str&        name  (     )C {return _name;} // get Joypad name
          CChar8* buttonName  (Int b)C;                // get button name  , buttonName  (JB_A) -> "A", buttonName  (JB_DPAD_UP) -> "Up"
   static CChar8* ButtonName  (Int b);                 // get button name  , ButtonName  (JB_A) -> "A", ButtonName  (JB_DPAD_UP) -> "Up"
          CChar * buttonSymbol(Int b)C;                // get button symbol, buttonSymbol(JB_A) -> "A", buttonSymbol(JB_DPAD_UP) -> "⯅", Warning: this function might return "⯇⯈⯆⯅△□○✕", if you want to display symbols on the screen be sure to include these characters in your Font
   static CChar * ButtonSymbol(Int b);                 // get button symbol, ButtonSymbol(JB_A) -> "A", ButtonSymbol(JB_DPAD_UP) -> "⯅", Warning: this function might return "⯇⯈⯆⯅△□○✕", if you want to display symbols on the screen be sure to include these characters in your Font
            Bool        mini  (     )C {return _mini;} // if  this is a mini Joypad (a single Nintendo Switch Joy-Con Left or Right held horizontally)

   Joypad& vibration(C Vec2 &vibration                    ); // set vibrations, 'vibration.x'=left motor intensity (0..1), 'vibration.y'=right motor intensity (0..1)
   Joypad& vibration(C Vibration &left, C Vibration &right); // set vibrations

 C Vec   & accel ()C {return _sensor_right.accel;} // get accelerometer value, available only if 'supportsSensors'
 C Vec   & gyro  ()C {return _sensor_right.gyro ;} // get gyroscope     value, available only if 'supportsSensors'
 C Orient& orient()C {return _sensor_right.orn  ;} // get orientation   value, available only if 'supportsSensors'

 C Sensor& sensorLeft ()C {return _sensor_left ;} // get sensor of the left  part of the Joypad, available only if 'supportsSensors'
 C Sensor& sensorRight()C {return _sensor_right;} // get sensor of the right part of the Joypad, available only if 'supportsSensors'

 C Color2& colorLeft ()C {return _color_left ;} // get color of the left  part of the Joypad (TRANSPARENT if unknown)
 C Color2& colorRight()C {return _color_right;} // get color of the right part of the Joypad (TRANSPARENT if unknown)

private:
   BS_FLAG _button[32];
   Bool    _mini=false;
   Bool    _connected=false;
#if JP_GAMEPAD_INPUT
   Bool    _vibrations=false;
#endif
#if JP_GAMEPAD_INPUT || JP_X_INPUT || JP_DIRECT_INPUT
   Bool    _state_index=false;
#endif
   Byte    _joypad_index;
   Byte    _name_type=0;
#define JOYPAD_BUTTON_REMAP (JP_DIRECT_INPUT || JP_GAMEPAD_INPUT || MAC || ANDROID)
#if     JOYPAD_BUTTON_REMAP
   Byte    _remap[32];
#endif
#if JP_GAMEPAD_INPUT || ANDROID
   Byte    _axis_stick_r_x=0xFF, _axis_stick_r_y=0xFF, _axis_trigger_l=0xFF, _axis_trigger_r=0xFF;
#endif
#if JP_DIRECT_INPUT
   Byte    _axis_stick_r_x=0, _axis_stick_r_y=0;
#endif
#if JP_X_INPUT
   Byte    _xinput=255;
#endif
#define JOYPAD_VENDOR_PRODUCT_ID (JP_DIRECT_INPUT && JP_GAMEPAD_INPUT) // needed only if using both JP_DIRECT_INPUT and JP_GAMEPAD_INPUT
#if     JOYPAD_VENDOR_PRODUCT_ID
   U16     _vendor_id=0, _product_id=0;
#endif
#if JP_GAMEPAD_INPUT
   Int     _buttons=0, _switches=0, _axes=0;
#endif
   UInt    _id=0;
#if SWITCH
   typedef UInt Buttons;
   UInt    _vibration_handle[2], _sensor_handle[2];
   Buttons _state_buttons=0;
   Long    _sampling_number=-1;
#endif
   Flt     _last_t[32], _dir_t, _dir_at[2];
   Color2  _color_left, _color_right;
#if ANDROID
   Vec2    _axis_trigger_mad[2]; // x=mul, y=add, [0]=l, [1]=r
#endif
   Sensor  _sensor_left, _sensor_right;
   Str     _name;
#if JP_DIRECT_INPUT
   Ptr     _dinput=null;
#endif
#if JP_GAMEPAD_INPUT
   Ptr     _gamepad=null, _raw_game_controller=null;
#endif
#if MAC
   struct Elm
   {
      enum TYPE : Byte
      {
         PAD   ,
         BUTTON,
         AXIS  ,
      };
      TYPE               type;
      Byte               index;
      UInt               cookie;
      Int                avg, max; // button_on=(val>avg);
      Flt                mul, add;
   };

   Mems<Elm> _elms;
   Ptr       _device=null;
#elif IOS
   struct Elm
   {
      enum TYPE : Byte
      {
         PAD    ,
         BUTTON ,
         TRIGGER,
         AXIS   ,
      };
      TYPE                 type;
      Byte                 index;
      Ptr                  element;
   };
   Mems<Elm> _elms;
   Ptr     _gamepad=null
#endif

   struct State
   {
   #if (JP_GAMEPAD_INPUT && WINDOWS_OLD) || JP_X_INPUT || JP_DIRECT_INPUT
      union
      {
         struct Data
         {
            UInt data[(32 + 4 + 6*8)/4]; // use UInt to force alignment
         }data;
      };
   #endif

   #if JP_GAMEPAD_INPUT && WINDOWS_NEW
      Ptr  button=null, Switch=null, axis=null;
      UInt gamepad[16];
   #endif
   }_state[2];

  ~Joypad();
   Joypad();

   NO_COPY_CONSTRUCTOR(Joypad);
};
/******************************************************************************/
struct JoypadsClass // Container for Joypads
{
   Int     elms      (       )C {return _data.elms() ;} // number of Joypads
   Joypad& operator[](Int  i )  {return _data     [i];} // get i-th Joypad
   Joypad* addr      (Int  i )  {return _data.addr(i);} // get i-th Joypad      , null on fail
   Joypad* find      (UInt id);                         // find Joypad by its ID, null on fail

   void swapOrder(Int i  , Int j        ); // swap order of 'i' and 'j' joypads
   void moveElm  (Int elm, Int new_index); // move 'elm' joypad to new position located at 'new_index'

private:
   MemtN<Joypad, 4> _data;
}extern
   Joypads;
/******************************************************************************/
void ApplyDeadZone(Vec2 &v, Flt dead_zone); // apply dead zone to analog direction vector, 'dead_zone'=0..1, this can be optionally called per-frame for 'Joypad.dir_a' vectors

void JoypadSensors(Bool calculate); // if want Joypad sensors to be calculated (accelerometer, gyroscope, orientation)
Bool JoypadSensors();               // if want Joypad sensors to be calculated (accelerometer, gyroscope, orientation)

void ConfigureJoypads(Int min_players, Int max_players, C CMemPtr<Str> &player_names=null, C CMemPtr<Color> &player_colors=null); // open OS joypad configuration screen, 'min_players'=minimum number of players required, 'max_players'=maximum number of players allowed, 'player_names'=names for each player (optional), 'player_colors'=colors for each player (optional), supported only on Nintendo Switch

inline Int Elms(C JoypadsClass &jps) {return jps.elms();}
/******************************************************************************/
