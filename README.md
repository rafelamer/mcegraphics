# Introduction to Lossy Image Compression

I'm teaching a course about mathematics and computer science at the Technical University of Catalonia BarcelonaTech and I started a project about the basic concepts of lossy image compression.

One of the goals of the project was that my students could study the code and that this was as simple as possible, so I discarde more complex and powerfull programs and libraries. Then I wrote my own library for educational purposes.

## Getting Started

### Prerequisites

To compile and install the library you need a Unix-like computer (I have tested it in Debian and Ubuntu) with a compiler (GCC) and the libraries zlib, netpbm and libfftw3

### Installing

To install the library, you have to run the following commands
```
~$ git clone https://github.com/rafelamer/mcegraphics.git
~$ cd mcegraphics
~$ make
~$ sudo make install
```
To install the programs, you have to do
```
~$ cd theprogram
~$ make
~$ sudo make install
```

### Running the tests

The folder *tests*, there are differets test programs that I used to test the library. To compile them, you can run
```
~$ cd tests
~$ make
```

## Author

* **Rafel Amer**
ESEIAAT
Technical University of Catalonia BarcelonaTech
rafel.amer@upc.edu


## License

This project is licensed under the GNU Lesser General Public License.  See the [LICENSE.md](LICENSE.md) file for details

## Acknowledgments

The author of the excellent book

- *Programming Projects in C for Students of
Engineering, Science and Mathematics*
Rouben Rostamian
SIAM
Computational Science & Engineering (2014)
ISBN 978-1-611973-49-5

You can also visit [https://en.wikipedia.org/wiki/JPEG](https://en.wikipedia.org/wiki/JPEG) 