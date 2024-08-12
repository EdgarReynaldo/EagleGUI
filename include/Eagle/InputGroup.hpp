
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
 * @file InputGroup.hpp
 * @brief This file contains the interface for the InputGroup class, an input tree built to process boolean expresssions.
 * 
 */

#ifndef EagleInputGroup_HPP
#define EagleInputGroup_HPP

#include <vector>
#include <string>
#include <iostream>

#include "Eagle/Input.hpp"

/**! @class InputGroup
 *   @brief Tree class for logical grouping of Input evaluations and storage 
 *
 *   How to implement the && || ! logic and chaining? With an object tree 
 *
 *   Trying to get a bool value from an empty input group will throw an exception, as
 *   returning true or false would be incorrect, since there's no logical test to evaluate 
 */

class InputGroup {
private :

   bool all_apply;///< True means this is operator AND - False means any apply (operator OR)
   bool is;       ///< False means is not (operator NOT)

   std::vector<Input>      inputs;///< Collection of inputs to test
   std::vector<InputGroup> nodes;///< Collection of input group nodes to test

   mutable int node_level;///< Our node level

   void SetNodeLevel(int level) const;///< Set the node level

   bool IS(bool basis) const {return (is?basis:!basis);}///< Applies @ref is to @param basis

   bool SameTypeAs(const InputGroup& ig) {return ((all_apply == ig.all_apply) && (is == ig.is));}///< True if both sides are the same logic type

   /**! @fn operator==
    *   @brief Hidden equivalence operator, not public
    *   
    *   TODO : Would equivalence operators be useful?
    *   TODO : InputGroups would probably have to be sorted first 
    */
   bool operator==(const InputGroup& ig) const {(void)ig;return false;}

public :
   /** Constructors */
   
   /** @fn InputGroup::InputGroup
    *  @brief Constructor for InputGroup objects
    *  @param i An Input object
    *  @param ig An InputGroup object
    *  @param use_and True if this is a logical AND, false if this is logical OR
    *  @param use_is  True if this is a positive statement, false if this is logical NOT
    */
   
   InputGroup();
   InputGroup(const Input& i);
   InputGroup(const InputGroup& ig);
   explicit InputGroup(bool use_and , bool use_is , const Input& i);
   explicit InputGroup(bool use_and , bool use_is , const InputGroup& ig);

   InputGroup& operator=(const InputGroup& ig);
   
   /** Vector like behavior */
   
   void        clear();///< Clear this input group of inputs
   InputGroup& push_back(const Input& i);///< Add an Input object to this group
   InputGroup& push_back(const InputGroup& ig);///< Add an InputGroup object to this group

   /** Logical grouping behavior */ /* TODO : Check that grouping of similar types groups inputs and groups */

   InputGroup operator&&(const InputGroup& ig) const ;///< Apply logical operator AND to *this and @param ig
   InputGroup operator||(const InputGroup& ig) const ;///< Apply logical operator OR to *this and @param ig
   InputGroup operator!() const ;///< Apply logical operator NOT to this group

   /** Evaluation - Don't evaluate empty input groups! */
   operator bool() const;///< Will throw an exception if this input group is empty

   /** If it makes you feel better to call a function to evaluate the expression instead
      of just using "if (InputGroup object) {...}", then here you go : */
   bool operator()() const {return (bool)(*this);}

   /**! @fn PointerInput
    *   @brief True if this input group tests for a pointer input click
    *
    *   Small utility function for checking whether the input is from a pointer source - it
    *   checks whether the input is from the mouse or joystick as well as being a button click.
    *   As long as there is at least one mouse or joystick click input, this will return true. 
    */

   bool PointerInput() const;
   
   bool HasClickInput() const;///< True if there is a click input stored in this group

   /**! @fn FindClickInput
    *   @brief Returns the click input stored in this group or else 
    *          returns input_key_press(EAGLE_KEY_NONE) if there is no click input in the group
    */
   
   Input FindClickInput() const;
   
   /** Text logging */

   std::ostream& DescribeTo(std::ostream& os , Indenter indent = Indenter()) const ;///< Describe this InputGroup to a stream

   friend std::ostream& operator<<(std::ostream& os , const InputGroup& ig);///< Friend function for stream output

   std::ostream& ShowLogic(std::ostream& os) const ;///< Show the logic in text outputting it to a stream
   
   void WriteLines(std::vector<std::string>* strvec);///< Store the logic lines in a vector of strings
};



/** Global operators to create InputGroup objects from logically joined Inputs */

/**! @fn operator&& <const Input& , const Input&>
 *   @fn operator|| <const Input& , const Input&>
 *   @fn operator! <const Input&>
 *   @brief Global logical operators for working with Input objects
 *   @retval An InputGroup object storing the specified inputs
 */

InputGroup operator&&(const Input& i1 , const Input& i2);
InputGroup operator||(const Input& i1 , const Input& i2);
InputGroup operator!(const Input& i);

/**! @fn operator&& <const Input& , const InputGroup&>
 *   @fn operator|| <const Input& , const InputGroup&>
 *   @brief Global operators for working with Input and InputGroup objects.
 *   
 *   Input and InputGroup objects may be combined using logical operators to
 *   create a composite InputGroup object that will test for all the conditions specified in its inputs
 *   @retval A composite InputGroup object
 */

InputGroup operator&&(const Input& i , const InputGroup& ig);
InputGroup operator||(const Input& i , const InputGroup& ig);


#endif // EagleInputGroup_HPP
