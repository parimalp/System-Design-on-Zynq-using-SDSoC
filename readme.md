# System Design on Zynq using SDSoC

The following labs provide an hands-on experience of creating application-specific systems on chip from C/C++ programs using the SDSoC development environment.

## Source Files Setup

To use the source files for each of the labs in this workshop, you have to clone this repository from Github. For that, do the following:

  If in **Windows**, download [Cmder](http://cmder.net/) or download [Git Bash](https://git-scm.com/download/win)

  If in **Ubuntu (or any Debian distribution)**, install git from the aptitude package manager by running this command in a terminal window:
  ```
  sudo apt-get install git
  ```

  After installing, set up git from [here](https://help.github.com/articles/set-up-git/).  Then open a new git-bash/cmder/terminal window and run the following command:
  ```
  git clone https://github.com/xupgit/System-Design-on-Zynq-using-SDSoC.git
  ```
In the instructions for the labs;

**{sources}** refers to the location **<clone directory>/sources/{board}_source**. Where {board} is either pynq_z1 or pynq_z2.

**{labs}** refers to the location which you will use as your workspace for the labs in the workshop. The recommended location is c:\xup\SDSoC\labs.

---

## Labs Overview:

### Lab 1

This lab guides you through the process of using SDx to create a new SDSoC project using available templates, marking functions for hardware implementation, building a hardware implemented design, and running the design on either the PYNQ-Z1 or PYNQ-Z2 board.

### Lab 2

This lab guides you through the process of handling data transfers between the software and hardware accelerators using various pragmas and the SDx API.

### Lab 3

Program hot-spots that are compute-intensive may be good candidates for hardware acceleration, especially when it is possible to stream data between hardware and the CPU and memory and overlap the computation with the communication. This lab guides you through the process of profiling an application, analyzing the results, identifying function(s) for hardware implementation, and then profiling again after targeting function(s) for acceleration.

### Lab 4

This lab guides you through the steps involved in estimating the expected performance of an application when functions are implemented in hardware, without going through the entire build cycle. You will further analyze how data movement is taking place by inserting an events tracer..

### Lab 5

This lab guides you through the steps involved in debugging a software application in SDSoC. SDSoC supports Standalone and Linux application debugging. SDSoC also provides the Dump/Restore Data File feature which can be used to dump a memory snapshot on a disk and restore the memory content from a pre-defined file. SDSoC also provides QEMU emulation capabilities which can be used for hardware/software debugging. 

### Lab 6

This lab introduces various techniques and directives of Vivado HLS which can be used in SDx to improve design performance. The design under consideration performs a discrete cosine transformation (DCT) on an 8x8 block of data.

### Lab 7

This lab guides you through the steps of creating and testing a custom platform.
