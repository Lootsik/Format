#pragma once
#include <iostream>
#include <sstream>



template<typename Str,typename Stream> 
	class format_base
{
public:

	using string_type = Str;
	using stream_type = Stream;




	format_base(const string_type& in_format)
					:format{ in_format }{}
 


	format_base(const format_base& another) = default;
	format_base& operator=(const format_base& another) = default;


	
	format_base(format_base&& another)
	{
		move_from(std::move(another));
	}

	format_base& operator=(format_base&& another)
	{
		move_from(std::move(another));
	}





	const string_type& get_string() const {
		return format;
	}

	const stream_type& get_stream() const {
		return intermediate_stream;
	}






	template<typename OStream>
	friend OStream& operator<<(OStream& os, const format_base& f)
	{
		if (f.no_insertions())
			os << f.format;
		else {
			if (f.unchecked_exist())
				f.print_unchecked();

			os << f.intermediate_stream.str();
		}
		return os;
	}

	// because member function needed to call whith temporary object
	template<typename T>
	format_base<Str, Stream>& operator%(const T& var)
	{
		if (next_insert_mark()) {
			intermediate_stream << var;
		}
		return *this;
	}


private:

	static const char insert_mark = '%';


	bool next_insert_mark()
	{
		if (!unchecked_exist())
			return false;

		size_t size = format.size();

		auto begin_write = format.c_str() + cur_pos;
		size_t begin_part_index = cur_pos;

		// finding next insert_mark or end
		for (; cur_pos < size && format[cur_pos] != format_base::insert_mark; cur_pos++);

		size_t bytes_to_write = cur_pos - begin_part_index;

		intermediate_stream.write(begin_write, bytes_to_write);

		// no insert_marks
		if (cur_pos == size)
			return false;

		++cur_pos;
		return true;
	}



	bool unchecked_exist() const
	{
		return cur_pos != format.size();
	}



	void print_unchecked() const
	{
		intermediate_stream.write(format.c_str() + cur_pos, format.size() - cur_pos);
	}



	bool no_insertions() const
	{
		return cur_pos == 0;
	}

	void move_from(format_base&& another)
	{
		format = std::move(another.format);
		intermediate_stream = std::move(another.intermediate_stream);
		cur_pos = another.cur_pos;
		another.cur_pos = 0;
	}	





	string_type	format;

	mutable stream_type intermediate_stream;

	size_t cur_pos = 0;
};


using format  = format_base<std::string, std::ostringstream>;
using formatw = format_base<std::wstring, std::wostringstream>;

inline format Fmt(const char* str)
{
	return format{str};
}

inline formatw Fmt(const wchar_t* str)
{
	return formatw{str};
}


