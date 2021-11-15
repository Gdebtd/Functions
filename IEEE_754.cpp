#include <iostream>
#include <string>

using namespace std;

char to_str(int num)
{
    return '0' + num;
}

int get_abs(int num) // returns module number
{
	if (num < 0)
		num *= -1;
	return num;
}

int get_size(string str) // returns size of string
{
	int i = 0;
	while (str[i] != '\0')
		i++;
	return i;
}

int get_len(int num, int del) // returns length of number (deviding by del)
{
	int i = 0;
	if (num == 0)
		i = 1;
	while (num != 0)
	{
		num /= del;
		i++;
	}
	return i;
}

bool have_dot(string str) // returns 1 if string have '.' and 0 if haven't
{
	for (int i = 0; i < get_size(str); i++)
	{
		if (str[i] == '.')
			return true;
	}
	return false;
}

string get_reverse_str(string str) // returns reversed string
{
	string result = "";
	for (int i = get_size(str) - 1; i >= 0; i--)
	{
		result += str[i];
	}
	return result;
}

int get_exponent(string str) // returns bin form of exponent (8 bit)
{
	int dotPos = 0;
	int onePos = 0;
	int _size = get_size(str);
	int i = 0;
	while (i < _size) // get position of '.'
	{
		if (str[i] == '.')
		{
			dotPos = i;
			break;
		}
		i++;
	}
	i = 0;
	int k = 0;
	while (i < _size) // get position of first '1'
	{
		if (str[i] != '.')
		{
			if (str[i] == '1')
			{
				onePos = k + 1; // position of new '.' will be equal to position of '1' + 1
				break;
			}
			k++;
		}
		i++;
	}
	return dotPos - onePos;
}

string get_mantiss(string str) // returns bin form of mantiss (23 bits)
{
	string result = "";
	int i = 0;
	int k = 0;
	bool calculate = false;
	while (i < get_size(str) && k < 23)
	{
		if (calculate) // if first '1' was found, then calculating to the 23 bits
		{
			if (str[i] != '.')
			{
				result += str[i];
				k++;
			}
		}
		if (str[i] == '1') // find first '1'
			calculate = true;
		i++;
	}
	while (k < 23) // if now bits count less than 23, then adding '0' to answer
	{
		result += "0";
		k++;
	}
	return result;
}

string get_toBin_first(int num) // returns before dot bin form of number
{
	string result = "";
	int len = get_len(num, 2); // get length in 2nd system
	if (num == 0)
		result += "0";
	while (num != 0) // adding numbers to answer (not reversed)
	{
		result += to_str(num % 2);
		num /= 2;
	}
	result = get_reverse_str(result); // reversing answer
	return result;
}

string get_toBin_second(int num, int len) // returns after dot bin form of number
{
	string result = "";
	int k = 1;
	for (int i = 0; i < len; i++) // get pow(10, len)
		k *= 10;
	int i = 0;
	while (i < 100 && num != 0) // get result in 2nd system (not highter, then 100 bits)
	{
		// e.g. (1).75 -> (2).75 * 2 = 1.5 -> (3)adding 1 (because we have 1.5 and need (int)form) -> (4)1.5 % 1 = .5
		// algoritm calculats result using (int) => we adding 'k' using 'len'
		num *= 2;
		result += to_str(num / k);
		num %= k;
		i++;
	}
	return result;
}

string get_toBin(string str) // returns bin form of number
{
	string result = "";
	int i = 0;
	int num1 = 0, num2 = 0;
	int len2 = 0;
	bool afterDot = false;
	while (str[i] != '\0')
	{
		if (str[i] == '.') // if before '.', then first number, else - second
			afterDot = true;
		else
		{
			if (!afterDot) // first number
			{
				num1 *= 10;
				num1 += str[i] - '0';
			}
			else // second number
			{
				num2 *= 10;
				num2 += str[i] - '0';
				len2++;
			}
		}
		i++;
	}
	// result = bin form before dot + '.' + bin form after dot
	result = get_toBin_first(num1) + '.' + get_toBin_second(num2, len2);
	return result;
}

string get_IEEE_754(string str)
{
	string result = "";
	bool sign = false; // sign = 0
	if (str[0] == '-') // if number is negative
	{
		sign = true; // sign = 1
		string _str = str;
		str = "";
		for (int i = 1; i < get_size(_str); i++) // recalculate number without '-'
			str += _str[i];
	}
	if (!have_dot(str)) // is haven't '.', then we have to add it
	{
		str += ".0";
	}
	if (str == "0.0") // if number is zero
	{
		result = to_str(sign);
		for (int i = 0; i < 31; i++)
			result += '0';
	}
	else
	{
		str = get_toBin(str); // calculate bin form
		string e = "";
		for (int i = 0; i < 8 - get_len(127 + get_exponent(str), 2); i++) // calculate length of exponent
			e += '0';
		e += get_toBin_first(127 + get_exponent(str)); // calculate exponent
		string mantiss = get_mantiss(str); // calculate mantiss
		result = to_str((int)sign) + e + mantiss; // return IEEE-754 form of number
	}
	return result;
}

int main()
{
	string str;
	cin >> str;
	cout << get_IEEE_754(str);
	return 0;
}
