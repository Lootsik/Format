# Format #
Formatting library provide formatted input.


# Usage #
Include Format.h to your code.

# Tested compiler #
MSVC 19.12.25831, CLANG 3.8.1-24 and gcc 4.6.3

# Example # 
      std::cout << Fmt("% world!%") % "Hello" % '\n';
      
      // employee1 - insance of user defined class with overloaded operator<<
      std::cout << Fmt("Employee 1: % \n") % employee1;
      
Wide character example:

      std::wcout << Fmt("% % % %\n") % 1 % 2 % 3 % 4;
