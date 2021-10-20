/**
 * @file parser.c
 * @author Ivan Vnucec (https://github.com/IvanVnucec)
 * @brief
 * @version 0.1
 * @date 2020-02-13
 *
 * @copyright MIT Licence (c) 2020
 *
 */

#include "parser.h"

#include "main.h" /* for setPWM declaration */

#include <ctype.h>
#include <string.h>

/**
 * @brief           Here the User should put his functions in the same way as
 *                  these functions below.
 *
 * @param command   Pointer to the Command.
 *
 * @return PSR_Status_t PSR_OK
 *                      PSR_ERROR
 *
 * @note            Function is only visible in this file (it's defined in
 *                  wrapper_functions.c file).
 */
extern PSR_Status_t PSR_Call_function(const PSR_Command_t *command);

/**
 * @brief                   Compares the input string to the Parser and already
 *                          defined function name. See wrapper_functions.c file
 *                          for examples. The second argument must be with '\0'
 *                          terminated string.
 *
 * @param command           Command with function name and arguments in it.
 * @param input_func_name   Null terminated string.
 * @return PSR_Status_t     PSR_OK
 *                          PSR_ERROR
 *
 * @warning                 input_func_name must be c-style string (terminated
 *                          with '\0' character)!
 */
PSR_Status_t PSR_Compare_command(const PSR_Command_t *command,
                                 const char input_func_name[])
{
    char *ptr;

    /* Calculate the function name length */
    ptr = (char *)memchr(input_func_name, '\0', PSR_MAX_INPUT_FUNCTION_NAME_LENGTH);
    /* If input_func_name has no null terminator character '\0' */
    if (ptr == NULL) {
        return PSR_ERROR;
    }
    const int func_name_length = ptr - input_func_name;

    /* If the Function_names lengths are not equal */
    if (func_name_length != command->function_name_length) {
        return PSR_ERROR; /* Return failure */
    }

    /* If the function names are not equal */
    if (memcmp(command->function_name, input_func_name, func_name_length) != 0) {
        return PSR_ERROR;
    }

    return PSR_OK; /* Return Success */
}

/**
 * @brief Function which calls other functions based on the input strings.
 *
 * @param string        Input string to parse.
 * @param string_length Length of the string to parse. Length is without the '\0'.
 *                      Generally it is "string_length = strlen(some_string);".
 * @return PSR_Status_t PSR_OK
 *                      PSR_ERROR
 */
PSR_Status_t PSR_Parse_command(const char string[], int string_length)
{
    PSR_Command_t command;
    char *ptr1, *ptr2;

    /* ---------- Extract function name from string ---------- */
    /* Find first "(" */
    ptr1 = (char *)memchr(string, '(', string_length);
    /* If it doesn't exist then return Failure */
    if (ptr1 == NULL) {
        return PSR_ERROR; /* return failure */
    }

    command.function_name_length = ptr1 - string;
    /* If string length is greater than we can store */
    if (command.function_name_length > PSR_MAX_INPUT_FUNCTION_NAME_LENGTH) {
        return PSR_ERROR; /* Return failure */
    }

    /* Get a function name from the input string */
    memcpy(command.function_name, string, command.function_name_length);

    /* ---------- Extract function argument from input string ---------- */
    /* Skip opening parantesees '(' */
    ptr1 = ptr1 + 1;
    /* Find closing parantesees ')' */
    ptr2 = (char *)memchr(string, ')', string_length);
    /* If it doesn't exists then return failure */
    if (ptr2 == NULL) {
        return PSR_ERROR; /* Return failure */
    }

    command.argument_length = ptr2 - ptr1;
    /* If string length is greater than we can store */
    if (command.argument_length > PSR_MAX_ARGUMENT_LENGTH) {
        return PSR_ERROR; /* Return failure */
    }

    /* Get the argument from in-between parantesees */
    memcpy(command.argument, ptr1, command.argument_length);

    /* Call the appropriate function based on function name */
    return PSR_Call_function(&command);
}

/**
 * @brief               Converts the string type Command argument to integer.
 *
 * @param retval        Integer type of the argument string
 * @param command       Parser Command with function name and argument.
 * @return PSR_Status_t PSR_OK
 *                      PSR_ERROR
 */
PSR_Status_t PSR_Argument_to_int(int *retval, const PSR_Command_t *command)
{
    int i;
    int sign;

    /* If there are no arguments */
    if (command->argument_length == 0) {
        return PSR_ERROR;
    }

    i = 0;

    /* Check for sign */
    if (command->argument[i] == '-') {
        sign = -1;
        i++;
    } else {
        sign = 1;
    }

    *retval = 0;

    /* For every digit */
    for (; i < command->argument_length; i++) {
        /* If there is some non-digit char */
        if (!isdigit(command->argument[i])) {
            return PSR_ERROR; /* Return Failure */
        }

        *retval = *retval * 10 + (command->argument[i] - '0');
    }

    *retval = *retval * sign;

    return PSR_OK; /* Return Success */
}

/**
 * @brief               Converts the argument string to the float value.
 *
 * @param retval        Float value of the Argument string
 * @param command       Pointer to the Command with the string arguments.
 * @return PSR_Status_t PSR_OK
 *                      PSR_ERROR
 */
PSR_Status_t PSR_Argument_to_float(float *retval, const PSR_Command_t *command)
{
    const char *ptr;
    int counter;
    int sign;
    float value;
    float power;

    /* If there are no arguments */
    if (command->argument_length == 0) {
        return PSR_ERROR;
    }

    /* For preattier code format. */
    ptr = command->argument;

    counter = 0;

    /* If there is sign? */
    if (*ptr == '-') {
        sign = -1;
        ptr++;
        counter++;
    } else if (*ptr == '+') { /* works also for '+' */
        sign = 1;
        ptr++;
        counter++;
    } else {
        sign = 1;
    }

    /* Convert numbers left of the decimal point */
    value = 0.0f;
    while (isdigit(*ptr) && counter < command->argument_length) {
        value = value * 10.0f + (*ptr - '0');
        ptr++;
        counter++;
    }

    /* If there is decimal point and we didn't come to an end.*/
    if ((*ptr == '.' || *ptr == ',') && counter < command->argument_length) {
        ptr++;
        counter++;
    }

    /* Convert numbers right of the decimal point */
    power = 1.0f;
    /* While we didn't come to an end */
    while (counter < command->argument_length) {
        value = value * 10.0f + (*ptr - '0');
        power *= 10.0f;
        ptr++;
        counter++;
    }

    *retval = sign * value / power;

    return PSR_OK; /* Return success */
}

/**
 * @brief               Converts the parser Function arguments to the
 *                      "boolean" type value. Read the Warning below.
 *
 * @param retval        Integer 0 for '0' character argument
 *                      Integer 1 for non '0' character argument
 * @param command       Command with the function name and argument.
 * @return PSR_Status_t PSR_OK
 *                      PSR_ERROR
 * @warning             This is not exactly boolean return type but the
 *                      integer values 0 or 1.
 */
PSR_Status_t PSR_Argument_to_bool(int *retval, const PSR_Command_t *command)
{
    /* If there are no arguments */
    if (command->argument_length == 0) {
        return PSR_ERROR;
    }

    /* If Arg is equal to zero then return zero, one otherwise */
    if (command->argument[0] == '0') {
        *retval = 0;
    } else {
        *retval = 1;
    }

    return PSR_OK; /* Return Success */
}

// TODO [ivan vnucec]: Implement and put this function somewhere
void check_for_new_command(void)
{
    /*
    if (stringComplete == true) {
        // convert String to array of chars
        const char *inputString_c = inputString.c_str();

        PSR_Parse_command(inputString_c, inputString.length());

        inputString = "";    // clear the string:

        stringComplete = false;
    }
    */
}
