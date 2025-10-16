#ifndef GRAYTOOL_CLI_H
#define GRAYTOOL_CLI_H

#ifndef INPUT_MAX_LEN
	#define INPUT_MAX_LEN 2048
#endif

typedef enum cli_option
{
	CLI_OPTION_NONE = 0,
	CLI_OPTION_HELP,
	CLI_OPTION_VERSION,
	CLI_OPTION_OUTPUT,
	CLI_OPTION_AVERAGE,
	CLI_OPTION_LUMINANCE601,
	CLI_OPTION_LUMINANCE709,
	CLI_OPTION_LUMINANCE_CUSTOM,
	CLI_OPTION_DESATURATION,
	CLI_OPTION_VALUE
} cli_option;

void cli_version();
void cli_help(const char* executable);

cli_option cli_parse_option(const char* option);
int cli_run(int argc, const char** argv);

#endif // GRAYTOOL_CLI_H
