#ifndef BL_MEMBERFUNCTIONWRAPPER_HPP
#define BL_MEMBERFUNCTIONWRAPPER_HPP


//-------------------------------------------------------------------
// FILE:            blMemberFunctionWrapper.hpp
// CLASS:           blMemberFunctionWrapper
// BASE CLASS:      None
//
// PURPOSE:         A functor that wraps an object's member function
//                  so that it can easily be invoked by our timer
//
// AUTHOR:          Vincenzo Barbato
//                  http://www.barbatolabs.com
//                  navyenzo@gmail.com
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// NOTE: This class is defined within the blTimerAPI namespace
//-------------------------------------------------------------------
namespace blTimerAPI
{
//-------------------------------------------------------------------



//-------------------------------------------------------------------
template<typename blObjectType,
         typename blReturnType,
         typename...blFunctionArguments>

class blMemberFunctionWrapper
{
public: // Constructors and destructors



    // Constructor

    blMemberFunctionWrapper(blObjectType& object,
                            blReturnType (blObjectType::*method)(blFunctionArguments...))
                            : m_object(&object),
                              m_method(method)
    {
    }



    // Copy constructor

    blMemberFunctionWrapper(const blMemberFunctionWrapper<blObjectType,blReturnType,blFunctionArguments...>& event) = default;



    // Destructor

    ~blMemberFunctionWrapper()
    {
    }



public: // Public functions



    // Assignment operator

    blMemberFunctionWrapper&        operator=(const blMemberFunctionWrapper<blObjectType,blReturnType,blFunctionArguments...>& event) = default;



    // operator() used to invoke the member function
    // of the stored object

    blReturnType                    operator()(const blFunctionArguments&... functionParameters)
    {
        return ( (m_object->*m_method)(functionParameters...) );
    }



public: // Public variables



    // The resource and its method

    blObjectType*                   m_object;
    blReturnType                    (blObjectType::*m_method)(blFunctionArguments...);
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// End of namespace
}
//-------------------------------------------------------------------



#endif // BL_MEMBERFUNCTIONWRAPPER_HPP
