/**
 * @file  cmdline.h
 * @brief Command-line argument parsing module.
 */

#ifndef CMDLINE_H
#define CMDLINE_H



/**
 * @brief Initializes the command-line parsing module.
 *
 * This function must be invoked before any of the other functions.
 *
 * @param argc Number of command-line arguments.
 * @param argv Array of command-line arguments. The contents of this array are
 *             not modified in any manner.
 * @return 0 on success. If @a argc or @a argv is invalid, -1 is returned and @a
 *         errno is set to @c EINVAL.
 */
int cmdlineInit(int argc, char *argv[]);

/**
 * @brief Retrieves the program name.
 * @return The value of @c argv[0].
 */
const char *cmdlineGetProgramName(void);

/**
 * @brief Gets the corresponding value for a given option key (@a "-key=value"
 *        or @a "--key=value").
 *
 * If the same key appears several times in the command line, the value of its
 * latest occurrence is returned.
 *
 * @param key The key. Leading dashes may be omitted.
 * @return The value associated with the given key, or @c NULL if no such pair
 *         exists.
 */
const char *cmdlineGetValueForKey(const char key[]);


#endif /* CMDLINE_H */
