#pragma once

//--------------------------------------------------------------------
inline SFString StripTrailing(const SFString& str, char c)
{
	SFString ret = str;

	while (ret.endsWith(c))
		ret = ret.Left(ret.length()-1);

	return ret;
}

//--------------------------------------------------------------------
inline SFString StripLeading(const SFString& str, char c)
{
	SFString ret = str;

	while (ret.startsWith(c))
		ret = ret.substr(1);

	return ret;
}

//--------------------------------------------------------------------
inline SFString Strip(const SFString& str, char c)
{
	return StripTrailing(StripLeading(str, c), c);
}

//--------------------------------------------------------------------
inline SFString StripAny(const SFString& str, const SFString& any)
{
	SFString ret = str;
	while (endsWithAny(ret,any) || startsWithAny(ret,any))
	{
		for (int i=0;i<any.length();i++)
			ret = Strip(ret, any[i]);
	}
	return ret;
}
