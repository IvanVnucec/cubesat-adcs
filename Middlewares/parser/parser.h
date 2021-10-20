/**
 * @file parser.h
 * @author Ivan Vnucec (https://github.com/IvanVnucec)
 * @brief Parser Header file
 * @version 0.1
 * @date 2020-02-13
 *
 * @copyright MIT Licence (c) 2020
 *
 */

#ifndef __PARSER_H__
#define __PARSER_H__

#define PSR_MAX_INPUT_STRING_LENGTH 50
#define PSR_MAX_INPUT_FUNCTION_NAME_LENGTH 25
#define PSR_MAX_ARGUMENT_LENGTH \
    (PSR_MAX_INPUT_STRING_LENGTH - PSR_MAX_INPUT_FUNCTION_NAME_LENGTH)

enum PSR_Status_e { PSR_OK, PSR_ERROR };

typedef enum PSR_Status_e PSR_Status_t;

struct PSR_Command_s {
    char function_name[PSR_MAX_INPUT_FUNCTION_NAME_LENGTH];
    char argument[PSR_MAX_ARGUMENT_LENGTH];
    int function_name_length;
    int argument_length;
};

typedef struct PSR_Command_s PSR_Command_t;

PSR_Status_t PSR_Parse_command(const char string[], int string_length);
PSR_Status_t PSR_Argument_to_int(int *retval, const PSR_Command_t *command);
PSR_Status_t PSR_Argument_to_float(float *retval, const PSR_Command_t *command);
PSR_Status_t PSR_Argument_to_bool(int *retval, const PSR_Command_t *command);
PSR_Status_t PSR_Compare_command(const PSR_Command_t *command,
                                 const char input_func_name[]);

#endif /* __PARSER_H__ */
