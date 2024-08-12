
/**
 *
 *         _______       ___       ____      __       _______
 *        /\  ____\    /|   \     /  __\    /\ \     /\  ____\
 *        \ \ \___/_   ||  _ \   |  /__/____\ \ \    \ \ \___/_
 *         \ \  ____\  || |_\ \  |\ \ /\_  _\\ \ \    \ \  ____\
 *          \ \ \___/_ ||  ___ \ \ \ \\//\ \/ \ \ \____\ \ \___/_
 *           \ \______\||_|__/\_\ \ \ \_\/ |   \ \_____\\ \______\
 *            \/______/|/_/  \/_/  \_\_____/    \/_____/ \/______/
 *
 *
 *    Eagle Agile Gui Library and Extensions
 *
 *    Copyright 2009-2024+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 * @file Input.hpp
 * @brief This file contains the interface for the Input class, a state based boolean tester to simplify input.
 * 
 */

#ifndef EagleInput_HPP
#define EagleInput_HPP


#include <iostream>

#include "Eagle/Indenter.hpp"

#include "Eagle/InputHandler.hpp"



/**! @typedef INPUT_HANDLER
 *   @brief The basic typedef for an input handling function
 */

typedef bool (*INPUT_HANDLER)(int);

/**! @var input_handlers
 *   @brief An array of INPUT_HANDLER function objects for each input for each state
 */

extern INPUT_HANDLER input_handlers[NUM_INPUT_SRCS][NUM_INPUT_STATES];

/**! @var input_func_text
 *   @brief An array of text strings for each input for each state
 */

extern const char* input_func_text[NUM_INPUT_SRCS][NUM_INPUT_STATES];



/**! @class Input
 *   @brief The Input class is designed to test for input states. They are used as objects in boolean expressions to
 *          test whether the given input source and state was detected.
 *
 *   Since the Input class depends on the InputHandler, their state will not change between calls to @ref EagleInputHandler::HandleInputEvent
 */

class Input {

private :
   INPUTSRC src;///< @var src The source of the input
   INPUTSTATE state;///< @var state The state in question
   int value;///< @var The value for the button

   INPUT_HANDLER input_handler_func;///< Our input test function


public :
   /** Constructors */

   Input();///< Empty default constructor
   Input(INPUTSRC source , INPUTSTATE inputstate , int input_value);///< Constructor that takes an input source, state, and button value

   /** Reassignment */

   void AssignTo(INPUTSRC source , INPUTSTATE inputstate , int input_value);///< Reassign this Input to another test state

   /** Present time evaluation of input (NOTE : won't change until UpdateInput is called) */

   operator bool() const;///< Cast to bool for evaluation
   operator int() const;///< May cast to int for evaluation. Non-zero is true.

   /** operator! can't be used for evaluation, it needs to return an InputGroup with NOT set */
   //  bool operator!() {return !(bool)(*this);} /*      (Implemented globally below)      */

   /** So you can compare Input objects */
   bool operator==(const Input& i);///< True if equivalent
   bool operator!=(const Input& i);///< True if not equivalent

   /** Text output */

   std::ostream& DescribeTo(std::ostream& os , Indenter indent = Indenter()) const ;///< Describe this Input object to a stream

   friend std::ostream& operator<<(std::ostream& os , const Input& input);///< Friend global stream operator

   std::ostream& Shortname(std::ostream& os) const ;///< Get a short name for this Input object

   /** Getters */
   INPUTSRC Source() const {return src;}///< Return the source in use
   INPUTSTATE State()  const {return state;}///< Return the state to test for
   int Value()  const {return value;}///< Return the button value to test for
};


/**! @fn input_*
 *   @brief Inline functions to create an Input object corresponding to the given source, state, and passed value
 *   @param value The button you wish to check for
 *   @param joy_num The joystick you wish to test
 */

inline Input input_key_press(int value)    {return Input(KB , PRESS    , value);}
inline Input input_key_release(int value)  {return Input(KB , RELEASE  , value);}
inline Input input_key_held(int value)     {return Input(KB , HELD     , value);}
inline Input input_key_open(int value)     {return Input(KB , OPEN     , value);}
inline Input input_key_dblclick(int value) {return Input(KB , DBLCLICK , value);}

inline Input input_mouse_press(int value)    {return Input(MS , PRESS    , value);}
inline Input input_mouse_release(int value)  {return Input(MS , RELEASE  , value);}
inline Input input_mouse_held(int value)     {return Input(MS , HELD     , value);}
inline Input input_mouse_open(int value)     {return Input(MS , OPEN     , value);}
inline Input input_mouse_dblclick(int value) {return Input(MS , DBLCLICK , value);}

inline Input input_jstk_press(int joy_num , int value)    {return Input(INPUTSRC(JS1 + joy_num) , PRESS    , value);}
inline Input input_jstk_release(int joy_num , int value)  {return Input(INPUTSRC(JS1 + joy_num) , RELEASE  , value);}
inline Input input_jstk_held(int joy_num , int value)     {return Input(INPUTSRC(JS1 + joy_num) , HELD     , value);}
inline Input input_jstk_open(int joy_num , int value)     {return Input(INPUTSRC(JS1 + joy_num) , OPEN     , value);}
inline Input input_jstk_dblclick(int joy_num , int value) {return Input(INPUTSRC(JS1 + joy_num) , DBLCLICK , value);}

inline Input input_jstk1_press(int value)    {return Input(JS1 , PRESS    , value);}
inline Input input_jstk1_release(int value)  {return Input(JS1 , RELEASE  , value);}
inline Input input_jstk1_held(int value)     {return Input(JS1 , HELD     , value);}
inline Input input_jstk1_open(int value)     {return Input(JS1 , OPEN     , value);}
inline Input input_jstk1_dblclick(int value) {return Input(JS1 , DBLCLICK , value);}

inline Input input_jstk2_press(int value)    {return Input(JS2 , PRESS    , value);}
inline Input input_jstk2_release(int value)  {return Input(JS2 , RELEASE  , value);}
inline Input input_jstk2_held(int value)     {return Input(JS2 , HELD     , value);}
inline Input input_jstk2_open(int value)     {return Input(JS2 , OPEN     , value);}
inline Input input_jstk2_dblclick(int value) {return Input(JS2 , DBLCLICK , value);}

inline Input input_jstk3_press(int value)    {return Input(JS3 , PRESS    , value);}
inline Input input_jstk3_release(int value)  {return Input(JS3 , RELEASE  , value);}
inline Input input_jstk3_held(int value)     {return Input(JS3 , HELD     , value);}
inline Input input_jstk3_open(int value)     {return Input(JS3 , OPEN     , value);}
inline Input input_jstk3_dblclick(int value) {return Input(JS3 , DBLCLICK , value);}

inline Input input_jstk4_press(int value)    {return Input(JS4 , PRESS    , value);}
inline Input input_jstk4_release(int value)  {return Input(JS4 , RELEASE  , value);}
inline Input input_jstk4_held(int value)     {return Input(JS4 , HELD     , value);}
inline Input input_jstk4_open(int value)     {return Input(JS4 , OPEN     , value);}
inline Input input_jstk4_dblclick(int value) {return Input(JS4 , DBLCLICK , value);}

inline Input input_jstk5_press(int value)    {return Input(JS5 , PRESS    , value);}
inline Input input_jstk5_release(int value)  {return Input(JS5 , RELEASE  , value);}
inline Input input_jstk5_held(int value)     {return Input(JS5 , HELD     , value);}
inline Input input_jstk5_open(int value)     {return Input(JS5 , OPEN     , value);}
inline Input input_jstk5_dblclick(int value) {return Input(JS5 , DBLCLICK , value);}

inline Input input_jstk6_press(int value)    {return Input(JS6 , PRESS    , value);}
inline Input input_jstk6_release(int value)  {return Input(JS6 , RELEASE  , value);}
inline Input input_jstk6_held(int value)     {return Input(JS6 , HELD     , value);}
inline Input input_jstk6_open(int value)     {return Input(JS6 , OPEN     , value);}
inline Input input_jstk6_dblclick(int value) {return Input(JS6 , DBLCLICK , value);}

inline Input input_jstk7_press(int value)    {return Input(JS7 , PRESS    , value);}
inline Input input_jstk7_release(int value)  {return Input(JS7 , RELEASE  , value);}
inline Input input_jstk7_held(int value)     {return Input(JS7 , HELD     , value);}
inline Input input_jstk7_open(int value)     {return Input(JS7 , OPEN     , value);}
inline Input input_jstk7_dblclick(int value) {return Input(JS7 , DBLCLICK , value);}

inline Input input_jstk8_press(int value)    {return Input(JS8 , PRESS    , value);}
inline Input input_jstk8_release(int value)  {return Input(JS8 , RELEASE  , value);}
inline Input input_jstk8_held(int value)     {return Input(JS8 , HELD     , value);}
inline Input input_jstk8_open(int value)     {return Input(JS8 , OPEN     , value);}
inline Input input_jstk8_dblclick(int value) {return Input(JS8 , DBLCLICK , value);}



/**! @fn AnyInputPressed <Input*>
 *   @fn AnyKeyPressed <Input*>
 *   @fn NonModInputPressed <Input*>
 *   @fn ModifierHeld <Input*>
 *   @brief Test for the given input and store it in @param store
 *   
 *   store may be NULL for AnyKeyPressed if you don't wish to store the result, merely test if a key is pressed
 */

bool AnyInputPressed(Input* store);
bool AnyKeyPressed(Input* store = 0);
bool NonModInputPressed(Input* store);
bool ModifierHeld(Input* store);



#endif // EagleInput_HPP



