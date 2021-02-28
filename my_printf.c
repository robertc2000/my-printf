/* COMAN ROBERT - 323CB*/

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <math.h>

static int write_stdout(const char *token, int length)
{
	int rc;
	int bytes_written = 0;

	do {
		rc = write(1, token + bytes_written, length - bytes_written);
		if (rc < 0)
			return rc;

		bytes_written += rc;
	} while (bytes_written < length);

	return bytes_written;
}

static void swap(char *a, char *b)
{
	char c = *a;
	*a = *b;
	*b = c;

}

static void mirror(char *s)
{
	char* start = s, *end = s;
	while(*end != '\0')
		end++;
	end--;
	int l = strlen(s);
	for(int i = 0; i < l / 2; i++){
		swap(start, end);
		start++;
		end--;

	}
}

static int write_char(char c);

static int write_int(int x)
{
	if(x == 0){
		write_stdout("0", 1);
		return 1;
	}
	char* string = malloc(100);
	int i = 0, negative = 0;
	if(x < 0){
		write_char('-');
		negative++;
	}
	while(x){
		string[i] = abs((x % 10)) + '0';
		i++;
		x /= 10;
	}
	string[i] = '\0';
	mirror(string);
	int len = strlen(string);
	write_stdout(string, strlen(string));
	free(string);
	return len + negative;
}

static int write_unsigned_int(unsigned int x){
	if(x == 0){
		write_stdout("0", 1);
		return 1;
	}
	char* string = malloc(100);
	if(!string)
		exit(-1);
	int i = 0;
	while(x){
		string[i] = (x % 10) + '0';
		i++;
		x /= 10;
	}
	string[i] = '\0';
	mirror(string);
	write_stdout(string, strlen(string));
	int len = strlen(string);
	free(string);
	return len;
}

static int write_hex(unsigned int x){
	if(x == 0){
		write_stdout("0", 1);
		return 1;
	}
	char* string = malloc(100);
	int i = 0;
	while(x){
		int digit = x % 16;
		if(digit > 9)
			string[i] = digit - 10 + 'a';
		else
			string[i] = digit + '0';
		i++;
		x /= 16;
	}
	string[i] = '\0';
	mirror(string);
	write_stdout(string, strlen(string));
	int len = strlen(string);
	free(string);
	return len;
}

static int write_char(char c){
	char* string = malloc(1);
	if(!string)
		return 0;
	string[0] = c;
	write_stdout(string, 1);
	free(string);
	return 1;
}

static int write_string(char* string){
	for(int i = 0; i < strlen(string); i++)
		write_char(string[i]);
	return strlen(string);
}

int iocla_printf(const char *format, ...)
{
	va_list args;
	va_start(args, format);
	int nr_args = 0;
	char* s = (char*) format;
	// iterating through "format"
	while(*s != '\0'){
		if(s[0] == '%'){
			if(s[1] == 'd'){
				nr_args += write_int(va_arg(args, int));
			}
			else if(s[1] == 'u'){
				nr_args += write_unsigned_int(va_arg(args, unsigned int));
			}
			else if(s[1] == 'x'){
				nr_args += write_hex(va_arg(args, unsigned int));
			}
			else if(s[1] =='c'){
				nr_args += write_char(va_arg(args, int));
			}
			else if(s[1] == 's'){
				char* s = va_arg(args, char*);
				nr_args += write_string(s);
			}
			else if(s[1] == '%'){
				nr_args += write_char('%');
			}
			s += 2;
		}
		else{
			write_char(s[0]);
			s++;
			nr_args++;
		}

	}
	return nr_args;
}
