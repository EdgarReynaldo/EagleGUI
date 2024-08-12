
/**
 *
 *         _______       ___       ____      __       _______
 *        /\  ____\    /|   \     /  __\    /\ \     /\  ____\
 *        \ \ \___/_   ||  _ \   |  /__/____\ \ \    \ \ \___/_
 *         \ \  ____\  || |_\ \  |\ \ /\_  _\\ \ \    \ \  ____\
 *          \ \ \___/_ ||  ___ \ \ \ \\//\ \/ \ \ \____\ \ \___/_
 *           \ \______\||_|__/\_\ \ \ \_\/ |   \ \_____\\ \______\
 *            \/______/|/_/  \/_/  \_\____/     \/_____/ \/______/
 *
 *
 *    Eagle Agile Gui Library and Extensions
 *
 *    Copyright 2009-2024+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 * @file InputGroup.cpp
 * @brief Implementations of the InputGroup class and global operators to go with them.
 * 
 *
 */



#include <sstream>
#include <iostream>


 
#include "Eagle/InputGroup.hpp"



/** #################            InputGroup class methods               ################# */



void InputGroup::SetNodeLevel(int level) const {
   node_level = level;
   const int num_nodes = nodes.size();
   for (int i = 0 ; i < num_nodes ; ++i) {nodes[i].SetNodeLevel(level + 1);}
}



InputGroup::InputGroup() : all_apply(true) , is(true) , inputs() , nodes() , node_level(0) {}



InputGroup::InputGroup(const Input& i) : all_apply(true) , is(true) , inputs() , nodes() , node_level(0) {
   inputs.push_back(i);
}



InputGroup::InputGroup(const InputGroup& ig) :
            all_apply(ig.all_apply) , is(ig.is) , inputs() , nodes() , node_level(ig.node_level) {
/** TODO : where is this warning coming from? -
/mingw/include/c++/3.4.5/bits/stl_vector.h: In member function `std::vector<_Tp, _Alloc>& std::vector<_Tp, _Alloc>::operator=(const std::vector<_Tp, _Alloc>&) [with _Tp = InputGroup, _Alloc = std::allocator<InputGroup>]':
/mingw/include/c++/3.4.5/bits/stl_vector.h:715: warning: '__result' might be used uninitialized in this function
/mingw/include/c++/3.4.5/bits/stl_uninitialized.h:82: warning: '__cur' might be used uninitialized in this function
/mingw/include/c++/3.4.5/bits/stl_uninitialized.h:82: warning: '__cur' might be used uninitialized in this function
*/
   inputs = ig.inputs;
   nodes = ig.nodes;
}



InputGroup::InputGroup(bool use_and , bool use_is , const Input& i) :
            all_apply(use_and) , is(use_is) , inputs() , nodes() , node_level(0) {
   inputs.push_back(i);
}



InputGroup::InputGroup(bool use_and , bool use_is , const InputGroup& ig) :
            all_apply(use_and) , is(use_is) , inputs() , nodes() , node_level(0) {
   push_back(ig);
   /** Previous version, doesn't allow for consolidation 
   ig.SetNodeLevel(1);
   nodes.push_back(ig);
   */
}



InputGroup& InputGroup::operator=(const InputGroup& ig) {
   all_apply = ig.all_apply;
   is = ig.is;
   inputs = ig.inputs;
   nodes = ig.nodes;
   return *this;
}



void InputGroup::clear() {
   inputs.clear();
   nodes.clear();
}



InputGroup& InputGroup::push_back(const Input& i) {
   inputs.push_back(i);
   return *this;
}



InputGroup& InputGroup::push_back(const InputGroup& ig) {

   /** This function is Re-entrant */
   /** If the InputGroup to add is the same type as the current one,
         add their inputs to this InputGroup's input list and try to 
         add their nodes to this InputGroup's node list by re entering
         the function.
         If the InputGroup is not the same type then it needs to be added to
         the nodes list.
   */

   if (SameTypeAs(ig)) {
      const int num_new = ig.inputs.size();
      for (int i = 0 ; i < num_new ; ++i) {
         push_back(ig.inputs[i]);
      }
      const int num_new_groups = ig.nodes.size();
      for (int i = 0 ; i < num_new_groups ; ++i) {
         push_back(ig.nodes[i]);
      }
   } else {
      /* Original behavior */
      ig.SetNodeLevel(node_level + 1);
      nodes.push_back(ig);
   }

   return *this;
}


/** TODO : Working here - the behavior of && and || may be able to consolidate more inputs */

InputGroup InputGroup::operator&&(const InputGroup& ig) const {
   InputGroup clump(true , true , ig);
   clump.push_back(*this);
   return clump;
}



InputGroup InputGroup::operator||(const InputGroup& ig) const {
   InputGroup clump(false , true , ig);
   clump.push_back(*this);
   return clump;
}



InputGroup InputGroup::operator!() const {
   /** Return an InputGroup with the value of 'is' negated? Or a boolean value?      */
   /** Input group ==> " !(Input(A) || Input(B)) " ==> " !InputGroup(A || B) "       */
   /** As in, we don't want to leave a bool value in a grouping chain.               */
   /** This should still work with evaluations though, as "if (!InputGroup)" should  */
   /** end up as !InputGroup::operator(bool)().                                          */

   /* TODO : Can't this just return a reference to itself with the value of 'is' negated? */
   /*      - No, not for constant objects, so a new copy needs to be created and returned
   is = !is;
   return (*this);
   */
   InputGroup ig(*this);
   ig.is = !(ig.is);
   return ig;
}



InputGroup::operator bool() const {
   bool ret;
   const int num_inputs = inputs.size();
   const int num_nodes = nodes.size();
   if ((num_inputs + num_nodes) == 0) {
      return false;
      /// throw InputGroupError("Trying to evaluate an empty input group!");
   }
   if (all_apply) {
      ret = true;
      /** AND the inputs and nodes */
      for (int i = 0 ; i < num_inputs ; ++i) {
         /** If any are false, the whole AND group is false */
         if (!(bool)inputs[i]) {return IS(false);}
      }
      for (int i = 0 ; i < num_nodes ; ++i) {
         if (!(bool)nodes[i]) {return IS(false);}
      }
      
   } else {
      ret = false;
      /** OR the inputs and nodes */
      for (int i = 0 ; i < num_inputs ; ++i) {
         /** If any are true, the whole OR group is true */
         if (inputs[i]) {return IS(true);}
      }
      for (int i = 0 ; i < num_nodes ; ++i) {
         if (nodes[i]) {return IS(true);}
      }
   }
   return IS(ret);
}



bool InputGroup::PointerInput() const {
   for (unsigned int i = 0 ; i < inputs.size() ; ++i) {
      if ((inputs[i].Source() != KB) && (inputs[i].State() == PRESS)) {return true;}
   }
   for (unsigned int i = 0 ; i < nodes.size() ; ++i) {
      if (nodes[i].PointerInput()) {return true;}
   }
   return false;
}



bool InputGroup::HasClickInput() const {
   for (unsigned int i = 0 ; i < inputs.size() ; ++i) {
      if (inputs[i].State() == PRESS) {return true;}
   }
   for (unsigned int i = 0 ; i < nodes.size() ; ++i) {
      if (nodes[i].HasClickInput()) {return true;}
   }
   return false;
}



Input InputGroup::FindClickInput() const {
   const Input false_return = input_key_press(EAGLE_KEY_NONE);
   for (unsigned int i = 0 ; i < inputs.size() ; ++i) {
      if (inputs[i].State() == PRESS) {return inputs[i];}
   }
   for (unsigned int i = 0 ; i < nodes.size() ; ++i) {
      if (nodes[i].FindClickInput() != false_return) {return nodes[i].FindClickInput();}
   }
   return false_return;
}



std::ostream& InputGroup::DescribeTo(std::ostream& os , Indenter indent) const {
   os << indent << "InputGroup [Logical ";
   os << (all_apply?"AND":"OR") << " , " << (is?"IS":"NOT") << "]" << std::endl;

   os << indent << "{" << std::endl;
   ++indent;
   const int num_inputs = inputs.size();
   const int num_nodes  = nodes.size();
   for (int i = 0 ; i < num_inputs ; ++i) {
      os << indent << inputs[i] << std::endl;
   }
   for (int i = 0 ; i < num_nodes ; ++i) {
      nodes[i].DescribeTo(os,indent);
   }
   --indent;
   os << indent << "}" << std::endl;
   return os;
}



std::ostream& operator<<(std::ostream& os , const InputGroup& ig) {
   return ig.DescribeTo(os);
/**
   Indenter indent(ig.node_level,3);
   Indenter indent_input(ig.node_level + 1 , 3);
   os << indent << "InputGroup [Logical ";
   os << (ig.all_apply?"AND":"OR") << " , " << (ig.is?"IS":"NOT") << "]" << endl;
   os << indent << "{" << endl;
   const int num_inputs = ig.inputs.size();
   const int num_nodes  = ig.nodes.size();
   for (int i = 0 ; i < num_inputs ; ++i) {
      os << indent_input << ig.inputs[i] << endl;
   }
   for (int i = 0 ; i < num_nodes ; ++i) {
      os << ig.nodes[i] << endl;
   }
   os << indent << "}" << endl;
   return os;
*/
}



std::ostream& InputGroup::ShowLogic(std::ostream& os) const {
   const int num_inputs = inputs.size();
   const int num_nodes = nodes.size();
///   const int num_items = inputs.size() + nodes.size();
///   int item_num = 0;
   
   if (!is) {os << "!";}
   os << "(";
   for (int i = 0 ; i < num_inputs ; ++i) {
      inputs[i].Shortname(os);
      if ((i + 1) != num_inputs) {
         if (all_apply) {
            os << " && ";
         } else {
            os << " || ";
         }
      }
///      ++item_num;
   }
   for (int i = 0 ; i < num_nodes ; ++i) {
      if (( num_inputs > 0) || (i > 0)) {
         if (all_apply) {
            os << " && ";
         } else {
            os << " || ";
         }
      }
      nodes[i].ShowLogic(os);
   }
   os << ")";
   return os;
}



void InputGroup::WriteLines(std::vector<std::string>* strvec) {
   std::ostringstream ostr;
   Indenter indent(node_level,3);
   Indenter indent_input(node_level + 1 , 3);
   ostr << indent << "InputGroup [Logical ";
   ostr << (all_apply?"AND":"OR") << " , " << (is?"IS":"NOT") << "]";
   strvec->push_back(ostr.str());
   ostr.str("");
   ostr << indent << "{";
   strvec->push_back(ostr.str());
   ostr.str("");

   const int num_inputs = inputs.size();
   const int num_nodes  = nodes.size();
   for (int i = 0 ; i < num_inputs ; ++i) {
      ostr << indent_input << inputs[i];
      strvec->push_back(ostr.str());
      ostr.str("");
   }
   for (int i = 0 ; i < num_nodes ; ++i) {
      nodes[i].WriteLines(strvec);
//      ostr << ig.nodes[i] << endl;
   }
   ostr << indent << "}";
   strvec->push_back(ostr.str());
   ostr.str("");
}




/** ######   Global operators for joining Input objects into InputGroup objects   ######## */



InputGroup operator&&(const Input& i1 , const Input& i2) {
   return InputGroup(true , true , i1).push_back(i2);
}



InputGroup operator||(const Input& i1 , const Input& i2) {
   return InputGroup(false , true , i1).push_back(i2);
}



InputGroup operator&&(const Input& i , const InputGroup& ig) {
   InputGroup ignew(ig);
   return (ignew && i);
}



InputGroup operator||(const Input& i , const InputGroup& ig) {
   InputGroup ignew(ig);
   /// TODO : This may be wrong? - fix it?.
   return (ignew || i);
}



InputGroup operator!(const Input& i) {
   return InputGroup(true , false , i);
}


