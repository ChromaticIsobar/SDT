/** @file SDTDSL.h
@defgroup dsl SDTDSL.h: Domain-Specific Language
Macros for reducing boilerplate code and
repetitions in the SDT code base
@{
*/

#ifndef SDT_DSL_H
#define SDT_DSL_H

/** @defgroup utils Utilities
Utility macros for other macros
@{ */

/** \cond */
#ifndef __CONCAT
#define __CONCAT(X, Y) X ## Y
#endif
#define _CONCAT(X, Y) __CONCAT(X, Y)
#define _STRINGIFY(X) #X
/** \endcond */

/** @brief Concatenate the expansions of two macros */
#define CONCAT(X, Y) _CONCAT(X, Y)

/** @brief Convert into string the expansion of a macro */
#define STRINGIFY(X) _STRINGIFY(X)

/** @} */
/** @defgroup typeconv Type Name Conversions
Utility macros for type conversions between C and JSON
@{ */

/** \cond */
#define JSON_TYPE_int integer
#define JSON_TYPE_long integer
#define JSON_TYPE_unsigned_int integer
#define JSON_TYPE_double double

#define C_TYPE_int int
#define C_TYPE_long long
#define C_TYPE_unsigned_int unsigned int
#define C_TYPE_double double
/** \endcond */

/** @brief Get the JSON type from type name
\param[in] TYPE The C type name (with underscores instead of spaces) */
#define JSON_TYPE(TYPE) CONCAT(JSON_TYPE_, TYPE)

/** @brief Get the C type from type name
\param[in] TYPE The C type name (with underscores instead of spaces) */
#define C_TYPE(TYPE) CONCAT(C_TYPE_, TYPE)

/** @} */
/** @defgroup autodef Automatic Interface Definitions
Macros for automatic generation of common functions on SDT types
@{ */

/** @defgroup accessors Accessors
Macros for generating setter and getter functions.
\par
You should define a macro @c SDT<TYPE>_<PROPERTY>_TYPE as the type name of the property for each property.
The type name must have no whitespaces (replace them with underscores).
E.g. to specify the type of the property @c SomeProperty of type @c SDTSomeObject as @c some @c type, you would write
\code{.c}#define SDTSomeObject_SomeProperty_TYPE some_type\endcode
You can then generate the setter declaration with
\code{.c}SDT_SETTER_H(SomeObject, SomeProperty)\endcode
This will be replaced with
\code{.c}extern void SDTSomeObject_setSomeProperty(const SDTSomeObject *x, some type value);\endcode
And you can generate the the getter declaration with
\code{.c}SDT_GETTER_H(SomeObject, SomeProperty)\endcode
This will be replaced with
\code{.c}extern some type SDTSomeObject_getSomeProperty(const SDTSomeObject *x);\endcode
\par
For automatic implementation of accessors, you should define a macro @c SDT<TYPE>_<PROPERTY>_FIELD as the field name
associated to a property.
E.g. to specify the field associated with the previously defined property @c SomeProperty as @c some_field,
you would write
\code{.c}#define SDTSomeObject_SomeProperty_FIELD some_field\endcode
You can then generate the setter with
\code{.c}SDT_SETTER(SomeObject, SomeProperty)\endcode
This will be replaced with
\code{.c}void SDTSomeObject_setSomeProperty(const SDTSomeObject *x, some type value) { x->some_field = value; }\endcode
And you can generate the the getter with
\code{.c}SDT_GETTER(SomeObject, SomeProperty)\endcode
This will be replaced with
\code{.c}some type SDTSomeObject_getSomeProperty(const SDTSomeObject *x) { return x->some_field; }\endcode
\par
To generate all accessors at once, and to generate the automatic implementation of JSON methods, you should define a
macro @c SDT<TYPE>_PROPERTIES(T, P).
The definition body should contain, for each property, a line @c P(T, @c <PROPERTY>).
E.g. to the define the interface of type @c SDTSomeObject with a two properties @c SomeProperty and @c AnotherProperty,
you would write
\code{.c}
#define SDTSomeObject_PROPERTIES(T, P) \
P(T, SomeProperty) \
P(T, AnotherProperty)
\endcode
@{ */

/** \cond */
#define SDT_TYPE_FULL(SDT_TYPE) CONCAT(SDT, SDT_TYPE)
#define SDT_PROPERTY_FIELD(SDT_TYPE, PROPERTY) CONCAT(CONCAT(SDT_TYPE_FULL(SDT_TYPE), _), CONCAT(PROPERTY, _FIELD))
#define SDT_PROPERTY_TYPE(SDT_TYPE, PROPERTY) CONCAT(CONCAT(SDT_TYPE_FULL(SDT_TYPE), _), CONCAT(PROPERTY, _TYPE))
#define SDT_PROPERTY_C_TYPE(SDT_TYPE, PROPERTY) C_TYPE(SDT_PROPERTY_TYPE(SDT_TYPE, PROPERTY))
#define SDT_PROPERTY_JSON_TYPE(SDT_TYPE, PROPERTY) JSON_TYPE(SDT_PROPERTY_TYPE(SDT_TYPE, PROPERTY))
#define SDT_FNAME(SDT_TYPE, PREFIX, PROPERTY) CONCAT(SDT_TYPE_FULL(SDT_TYPE), CONCAT(PREFIX, PROPERTY))
#define SDT_GETTER_FNAME(SDT_TYPE, PROPERTY) SDT_FNAME(SDT_TYPE, _get, PROPERTY)
#define SDT_SETTER_FNAME(SDT_TYPE, PROPERTY) SDT_FNAME(SDT_TYPE, _set, PROPERTY)
/** \endcond */

/** @brief Generate getter function declaration
\param[in] SDT_TYPE The SDT type name (without the SDT prefix)
\param[in] PROPERTY The type property name (as used in macro definitions) */
#define SDT_GETTER_H(SDT_TYPE, PROPERTY) \
extern SDT_PROPERTY_C_TYPE(SDT_TYPE, PROPERTY) SDT_GETTER_FNAME(SDT_TYPE, PROPERTY)( \
  const SDT_TYPE_FULL(SDT_TYPE) *x \
);

/** @brief Generate getter function declarations for all properties
\param[in] SDT_TYPE The SDT type name (without the SDT prefix) */
#define SDT_GETTERS_H(SDT_TYPE) CONCAT(SDT_TYPE_FULL(SDT_TYPE), _PROPERTIES)(SDT_TYPE, SDT_GETTER_H)

/** @brief Generate getter function implementation
\param[in] SDT_TYPE The SDT type name (without the SDT prefix)
\param[in] PROPERTY The type property name (as used in macro definitions) */
#define SDT_GETTER(SDT_TYPE, PROPERTY) \
SDT_PROPERTY_C_TYPE(SDT_TYPE, PROPERTY) SDT_GETTER_FNAME(SDT_TYPE, PROPERTY)( \
  const SDT_TYPE_FULL(SDT_TYPE) *x \
){ \
  return x-> SDT_PROPERTY_FIELD(SDT_TYPE, PROPERTY); \
}

/** @brief Generate getter function implementations for all properties
\param[in] SDT_TYPE The SDT type name (without the SDT prefix) */
#define SDT_GETTERS(SDT_TYPE) CONCAT(SDT_TYPE_FULL(SDT_TYPE), _PROPERTIES)(SDT_TYPE, SDT_GETTER)

/** @brief Generate setter function declaration
\param[in] SDT_TYPE The SDT type name (without the SDT prefix)
\param[in] PROPERTY The type property name (as used in macro definitions) */
#define SDT_SETTER_H(SDT_TYPE, PROPERTY) \
extern void SDT_SETTER_FNAME(SDT_TYPE, PROPERTY)( \
  const SDT_TYPE_FULL(SDT_TYPE) *x, \
  SDT_PROPERTY_C_TYPE(SDT_TYPE, PROPERTY) value \
);

/** @brief Generate setter function declarations for all properties
\param[in] SDT_TYPE The SDT type name (without the SDT prefix) */
#define SDT_SETTERS_H(SDT_TYPE) CONCAT(SDT_TYPE_FULL(SDT_TYPE), _PROPERTIES)(SDT_TYPE, SDT_SETTER_H)

/** @brief Generate setter function implementation
\param[in] SDT_TYPE The SDT type name (without the SDT prefix)
\param[in] PROPERTY The type property name (as used in macro definitions) */
#define SDT_SETTER(SDT_TYPE, PROPERTY) \
void SDT_SETTER_FNAME(SDT_TYPE, PROPERTY)( \
  const SDT_TYPE_FULL(SDT_TYPE) *x, \
  SDT_PROPERTY_C_TYPE(SDT_TYPE, PROPERTY) value \
){ \
  x-> SDT_PROPERTY_FIELD(SDT_TYPE, PROPERTY) = value; \
}

/** @brief Generate setter function implementations for all properties
\param[in] SDT_TYPE The SDT type name (without the SDT prefix) */
#define SDT_SETTERS(SDT_TYPE) CONCAT(SDT_TYPE_FULL(SDT_TYPE), _PROPERTIES)(SDT_TYPE, SDT_SETTER)

/** @brief Generate getter and setter function declarations
\param[in] SDT_TYPE The SDT type name (without the SDT prefix)
\param[in] PROPERTY The type property name (as used in macro definitions) */
#define SDT_ACCESSOR_H(SDT_TYPE, PROPERTY) SDT_GETTER_H(SDT_TYPE, PROPERTY) SDT_SETTER_H(SDT_TYPE, PROPERTY)

/** @brief Generate getter and setter function implementations
\param[in] SDT_TYPE The SDT type name (without the SDT prefix)
\param[in] PROPERTY The type property name (as used in macro definitions) */
#define SDT_ACCESSOR(SDT_TYPE, PROPERTY) SDT_GETTER(SDT_TYPE, PROPERTY) SDT_SETTER(SDT_TYPE, PROPERTY)

/** @brief Generate getter and setter function declarations for all properties
\param[in] SDT_TYPE The SDT type name (without the SDT prefix) */
#define SDT_ACCESSORS_H(SDT_TYPE) SDT_GETTERS_H(SDT_TYPE) SDT_SETTERS_H(SDT_TYPE)

/** @brief Generate getter and setter function implementations for all properties
\param[in] SDT_TYPE The SDT type name (without the SDT prefix) */
#define SDT_ACCESSORS(SDT_TYPE) SDT_GETTERS(SDT_TYPE) SDT_SETTERS(SDT_TYPE)

/** @} */
/** @} */
/** @} */

#endif
