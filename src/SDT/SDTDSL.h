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

#define _CCONCAT(X, Y) X ## Y
#define _CONCAT(X, Y) _CCONCAT(X, Y)
#define _STRINGIFY(X) #X

/** @brief Concatenate the expansions of two macros */
#define CONCAT(X, Y) _CONCAT(X, Y)

/** @brief Convert into string the expansion of a macro */
#define STRINGIFY(X) _STRINGIFY(X)

/** @} */
/** @defgroup typeconv Type Name Conversions
Utility macros for type conversions between C and JSON
@{ */

#define JSON_TYPE_int integer
#define JSON_TYPE_long integer
#define JSON_TYPE_unsigned_int integer
#define JSON_TYPE_double double

#define C_TYPE_int int
#define C_TYPE_long long
#define C_TYPE_unsigned_int unsigned int
#define C_TYPE_double double

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
/** @defgroup autotypedef Automatic Type Definitions
Macros for automatic generation of SDT type definitions and related functions
@{ */

#define SDT_TYPE_FULL(SDT_TYPE) CONCAT(SDT, SDT_TYPE)
#define SDT_FNAME(SDT_TYPE, PREFIX, PROPERTY) CONCAT(SDT_TYPE_FULL(SDT_TYPE), CONCAT(PREFIX, PROPERTY))
#define SDT_INIT_FNAME(SDT_TYPE) SDT_FNAME(SDT_TYPE, _new, )
#define SDT_FREE_FNAME(SDT_TYPE) SDT_FNAME(SDT_TYPE, _free, )

/** @brief Generate the type declaration
\param[in] SDT_TYPE The SDT type name (without the SDT prefix) */
#define SDT_TYPEDEF_H(SDT_TYPE) typedef struct SDT_TYPE_FULL(SDT_TYPE) SDT_TYPE_FULL(SDT_TYPE);

/** @brief Generate instantiation function declaration
\param[in] SDT_TYPE The SDT type name (without the SDT prefix) */
#define SDT_INIT_H(SDT_TYPE) extern SDT_TYPE_FULL(SDT_TYPE) *SDT_INIT_FNAME(SDT_TYPE)( \
  CONCAT(SDT_TYPE_FULL(SDT_TYPE), _SIGNATURE) \
);

/** @brief Generate destruction function declaration
\param[in] SDT_TYPE The SDT type name (without the SDT prefix) */
#define SDT_FREE_H(SDT_TYPE) extern void SDT_FREE_FNAME(SDT_TYPE)(SDT_TYPE_FULL(SDT_TYPE) *x);

/** @brief Generate type, initialization and destruction declarations
\param[in] SDT_TYPE The SDT type name (without the SDT prefix) */
#define SDT_AUTOTYPE_H(SDT_TYPE) SDT_TYPEDEF_H(SDT_TYPE) SDT_INIT_H(SDT_TYPE) SDT_FREE_H(SDT_TYPE)

/** @} */
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

#define SDT_PROPERTY_FIELD(SDT_TYPE, PROPERTY) CONCAT(CONCAT(SDT_TYPE_FULL(SDT_TYPE), _), CONCAT(PROPERTY, _FIELD))
#define SDT_PROPERTY_DEFAULT(SDT_TYPE, PROPERTY) CONCAT(CONCAT(SDT_TYPE_FULL(SDT_TYPE), _), CONCAT(PROPERTY, _DEFAULT))
#define SDT_PROPERTY_TYPE(SDT_TYPE, PROPERTY) CONCAT(CONCAT(SDT_TYPE_FULL(SDT_TYPE), _), CONCAT(PROPERTY, _TYPE))
#define SDT_PROPERTY_C_TYPE(SDT_TYPE, PROPERTY) C_TYPE(SDT_PROPERTY_TYPE(SDT_TYPE, PROPERTY))
#define SDT_PROPERTY_JSON_TYPE(SDT_TYPE, PROPERTY) JSON_TYPE(SDT_PROPERTY_TYPE(SDT_TYPE, PROPERTY))
#define SDT_GETTER_FNAME(SDT_TYPE, PROPERTY) SDT_FNAME(SDT_TYPE, _get, PROPERTY)
#define SDT_SETTER_FNAME(SDT_TYPE, PROPERTY) SDT_FNAME(SDT_TYPE, _set, PROPERTY)
#define SDT_PROPERTIES(SDT_TYPE) CONCAT(SDT_TYPE_FULL(SDT_TYPE), _PROPERTIES)

/** @brief Generate getter function declaration
\param[in] SDT_TYPE The SDT type name (without the SDT prefix)
\param[in] PROPERTY The type property name (as used in macro definitions) */
#define SDT_GETTER_H(SDT_TYPE, PROPERTY) \
extern SDT_PROPERTY_C_TYPE(SDT_TYPE, PROPERTY) SDT_GETTER_FNAME(SDT_TYPE, PROPERTY)( \
  const SDT_TYPE_FULL(SDT_TYPE) *x \
);

/** @brief Generate getter function declarations for all properties
\param[in] SDT_TYPE The SDT type name (without the SDT prefix) */
#define SDT_GETTERS_H(SDT_TYPE) SDT_PROPERTIES(SDT_TYPE)(SDT_TYPE, SDT_GETTER_H)

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
#define SDT_GETTERS(SDT_TYPE) SDT_PROPERTIES(SDT_TYPE)(SDT_TYPE, SDT_GETTER)

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
#define SDT_SETTERS_H(SDT_TYPE) SDT_PROPERTIES(SDT_TYPE)(SDT_TYPE, SDT_SETTER_H)

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
#define SDT_SETTERS(SDT_TYPE) SDT_PROPERTIES(SDT_TYPE)(SDT_TYPE, SDT_SETTER)

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
/** @defgroup autojson JSON De/serialization
Macros for generating JSON serialization and deserialization functions.
@{ */

#define JSON_integer_FIELD integer
#define JSON_double_FIELD dbl
#define JSON_TYPE_FIELD(TYPE) CONCAT(CONCAT(JSON_, TYPE), _FIELD)

#define SDT_JSON_PUSH(SDT_TYPE, PROPERTY) json_object_push( \
  obj, STRINGIFY(SDT_PROPERTY_FIELD(SDT_TYPE, PROPERTY)), \
  CONCAT(CONCAT(json_, SDT_PROPERTY_JSON_TYPE(SDT_TYPE, PROPERTY)), _new)( SDT_GETTER_FNAME(SDT_TYPE, PROPERTY)(x) ) \
);

#define SDT_JSON_PULL(SDT_TYPE, PROPERTY) \
const json_value *CONCAT(v_, PROPERTY) = json_object_get_by_key(x, STRINGIFY(SDT_PROPERTY_FIELD(SDT_TYPE, PROPERTY))); \
SDT_SETTER_FNAME(SDT_TYPE, PROPERTY)( y, \
  (CONCAT(v_, PROPERTY) && (CONCAT(v_, PROPERTY)->type == CONCAT(json_, SDT_PROPERTY_JSON_TYPE(SDT_TYPE, PROPERTY))))? \
  CONCAT(v_, PROPERTY)->u. JSON_TYPE_FIELD(SDT_PROPERTY_JSON_TYPE(SDT_TYPE, PROPERTY)) : \
  SDT_PROPERTY_DEFAULT(SDT_TYPE, PROPERTY)
);

/** @brief Generate a JSON serialization function declaration
\param[in] SDT_TYPE The SDT type name (without the SDT prefix) */
#define SDT_JSON_SERIALIZE_H(SDT_TYPE) extern json_value * CONCAT(SDT_TYPE_FULL(SDT_TYPE), _toJSON) ( \
  const SDT_TYPE_FULL(SDT_TYPE) *x \
);

/** @brief Generate a JSON serialization function implementation
\param[in] SDT_TYPE The SDT type name (without the SDT prefix)*/
#define SDT_JSON_SERIALIZE(SDT_TYPE) json_value * CONCAT(SDT_TYPE_FULL(SDT_TYPE), _toJSON) ( \
  const SDT_TYPE_FULL(SDT_TYPE) *x \
) { \
  json_value *obj = json_object_new(0); \
  SDT_PROPERTIES(SDT_TYPE)(SDT_TYPE, SDT_JSON_PUSH) \
  return obj; \
}

/** @brief Generate a JSON deserialization function declaration
\param[in] SDT_TYPE The SDT type name (without the SDT prefix)*/
#define SDT_JSON_DESERIALIZE_H(SDT_TYPE) \
extern SDT_TYPE_FULL(SDT_TYPE) *CONCAT(SDT_TYPE_FULL(SDT_TYPE), _fromJSON)(const json_value *x);

/** @brief Generate a JSON deserialization function implementation
\param[in] SDT_TYPE The SDT type name (without the SDT prefix)*/
#define SDT_JSON_DESERIALIZE(SDT_TYPE) \
SDT_TYPE_FULL(SDT_TYPE) *CONCAT(SDT_TYPE_FULL(SDT_TYPE), _fromJSON)(const json_value *x) { \
  if (!x || x->type != json_object) \
    return 0; \
  SDT_PROPERTIES(SDT_TYPE)(SDT_TYPE, SDT_JSON_PULL) \
  return y; \
}

/** @brief Generate function declarations for JSON de/serialization
\param[in] SDT_TYPE The SDT type name (without the SDT prefix) */
#define SDT_AUTOJSON_H(SDT_TYPE) SDT_JSON_SERIALIZE_H(SDT_TYPE) SDT_JSON_DESERIALIZE_H(SDT_TYPE)

/** @brief Generate function implementations for JSON de/serialization
\param[in] SDT_TYPE The SDT type name (without the SDT prefix) */
#define SDT_AUTOJSON(SDT_TYPE) SDT_JSON_SERIALIZE(SDT_TYPE) SDT_JSON_DESERIALIZE(SDT_TYPE)

/** @} */

/** @brief Generate function declarations for type con/destructors, accessors and JSON de/serialization
\param[in] SDT_TYPE The SDT type name (without the SDT prefix) */
#define SDT_AUTOAPI_H(SDT_TYPE) SDT_AUTOTYPE_H(SDT_TYPE) SDT_ACCESSORS_H(SDT_TYPE) SDT_AUTOJSON_H(SDT_TYPE)

/** @brief Generate function implementations for accessors and JSON de/serialization
\param[in] SDT_TYPE The SDT type name (without the SDT prefix) */
#define SDT_AUTOAPI(SDT_TYPE) SDT_ACCESSORS(SDT_TYPE) SDT_AUTOJSON(SDT_TYPE)

/** @} */
/** @} */

#endif
