#!/bin/sh
# Grading Guideline for Project 1
# ===============================

# Total points: 50

# +---------------------+
# | General Guidelines: |
# +---------------------+

# 1) The points below are maximum points.  Partial credits may be given.

# 2) Do the "plus points" first, then do the "minus points".

# 3) Each group's README will be read first to see if something different
#    needs to be done to get the code to work and what the known problems are.

# 4) The scripts below are for bash.

# 5) The grading is meant to be harsh!  So, if running the "diff" command
#    suppose to produce no output, but the group's code produces some
#    output, points will be taken off accordingly.  Similarly, if running
#    the "diff" command suppose to produce some output, but the group's
#    code produces no output, points will be taken off accordingly.

# 6) This grading guidelines is not perfect.  The grader will try his/her
#    best to stick to the grading guidelines.  Sometimes, after the grader
#    started grading some of the submissions, s/he realize that something
#    is missed (most often in "minus points").  The bottomline is that the
#    instructor reserves the right to add or change things in the grading
#    guidelines as he sees fit.

# +--------------+
# | Plus points: |
# +--------------+
 
# Documentation    : +3 points (comments in code plus README)

# (A) Hexdump : +15 points
    echo "OPTION HEXDUMP TEST"
	srcdir=testcases
    #
    # for the following commands, each correct answer gets 1 point
    #
    rm -f f?.hex f??.hex
    for f in 0 1 2 3 4 5 6 7 8
    do
        echo "===> $srcdir/f$f"
        ./proj1 hexdump $srcdir/f$f > f$f.hex
        diff $srcdir/f$f.hex f$f.hex
    done

    #
    # for the following commands, each correct answer gets 1 point
    #
    rm -f f?.hex f??.hex
    for f in 9 10 11 12 13 14
    do
        echo "===> $srcdir/f$f"
        cat $srcdir/f$f | ./proj1 hexdump > f$f.hex
        diff $srcdir/f$f.hex f$f.hex
    done

# (B) Base64 Encode : +11 points
    echo "OPTION ENC-BASE64 TEST"
	srcdir=testcases

    #
    # for the following commands, each correct answer gets 1 point
    #
    rm -f f?.b64
    for f in 5 6 7 8 9
    do
        echo "===> $srcdir/f$f"
        ./proj1 enc-base64 $srcdir/f$f > f$f.b64
        diff $srcdir/f$f.b64 f$f.b64
    done

    #
    # for the following commands, each correct answer gets 1 point
    #
    rm -f f?.b64
    for f in 0 1 2 3 4 9
    do
        echo "===> $srcdir/f$f"
        cat $srcdir/f$f | ./proj1 enc-base64 > f$f.b64
        diff $srcdir/f$f.b64 f$f.b64
    done

# (C) Base64 Decode : +11 points
    echo "OPTION DEC-BASE64 TEST"
	srcdir=testcases
    #
    # for the following commands, each correct answer gets 1 point
    #
    rm -f f?.dat
    for f in 5 6 7 9
    do
        echo "===> $srcdir/f$f"
        ./proj1 dec-base64 $srcdir/f$f.b64 > f$f.dat
        diff $srcdir/f$f f$f.dat
    done

    #
    # for the following commands, each correct answer gets 1 point
    #
    rm -f f?.dat
    for f in 0 1 2 3 9
    do
        echo "===> $srcdir/f$f"
        cat $srcdir/f$f.b64 | ./proj1 dec-base64 > f$f.dat
        diff $srcdir/f$f f$f.dat
    done

    #
    # for the following commands, each correct answer gets 1 point
    #
    rm -f f100.dat f101.dat
    for f in 100 101
    do
        echo "===> $srcdir/f$f"
        cat $srcdir/f$f.b64 | ./proj1 dec-base64 > f$f.dat
        diff $srcdir/f$f f$f.dat
    done

# (D) Base64 Encode and then Decode using student program : +10 points
    echo "WHOLE BASE64 PROCESS TEST"
	srcdir=testcases
    #
    # for the following commands, each correct answer gets 1 point
    #
    rm -f f?.dat
    for f in 0 1 2 3 4 5 6 7 8 9
    do
        echo "===> $srcdir/f$f"
        ./proj1 enc-base64 $srcdir/f$f | ./proj1 dec-base64 > f$f.dat
        diff $srcdir/f$f f$f.dat
    done

# +---------------+
# | Minus points: |
# +---------------+

# Missing README file  : -1 points (even if lots of comments in code)
#                        (Document+this cannot go negative)

# Improper citation    : -5 points
#                        for code obtained from a public source, the right place
#                        to cite the source is right next to the code

# Cannot compile       : -5 to -10, depending on effort to make it work
#                        if cannot generate executable, at least 5 points
#                        will be taken off, up to 10 points off if it
#                        takes a lot to make it work

# Compiler warnings    : -1 to -3
#                        if using gcc, make sure -Wall flag is used
#                        1 point will be taken off for 1-4 warnings,
#                        2 points off for 5-8 warnings, 3 points for more

# "make clean"         : -1 point if it does not work correctly

# Segmentation faults  : -10 points
#                        if a seg fault (or bus error) is seen any time during the
#                        testing of the code, if it's fairly repeatable, 10 points
#                        will be deducted, if it's intermittent, less will be deducted,
#                        depending on how often the seg faults are seen

# Using external library to do base64 encoding/decoding : -30 points
#                visually inspect the Makefile and see if the code is linked
#                with the crypto library or other libraries

# Buffer size violation : -10 points
#                visually inspect the code and look for malloc() or "new" and
#                see if a buffer whose size is greater than 1024 is used

# Separate compilation : -10 points
#                        if the executable is compiled with a single line, deduct
#                        all 10 points
#                        if the executable is a single module, but generates main.o
#                        and then link to create the executable in line one, deduct
#                        5 points
#                        if most of the code are in .h files, deduct all 10 points

# Too slow             : -10 points
#                        -2 points for each run that takes more than 5 minutes to complete

# Bad commandline : -1 point each for each not handling properly
                # need some sort of error output to indicate that the command
                #     cannot be completed, it does not have to exactly the
                #     error indicated below, just something reasonable to inform
                #     the user about the error condition
                # please note that commandline parsing is separate from
                #     other functionalities, so even though a student has
                #     declared that certain part of the assignments is not
                #     imlemented, commandling parsing still needs to be done
                #     for those commands
                ./proj1
                #    (malformed command)
                ./proj1 xyz
                #    (malformed command)
                ./proj1 abc def ghi
                #    (malformed command)
                ./proj1 -y hexdump
                #    (malformed command)
                ./proj1 hexdump /usr/bin/xyzz
                #    (input file /usr/bin/xyzz does not exist)

# Bad input data for dec-base64 : -1 point each for each not handling properly
                set srcdir=testcases

                #
                # just want to see if the program generates some good
                # error messages
                #
                ./proj1 dec-base64 $srcdir/f200.b64
                #    (should not generate anything but error messages)
                ./proj1 dec-base64 $srcdir/f201.b64
                #    (should not generate anything but error messages)
                ./proj1 dec-base64 $srcdir/f202.b64 > /dev/null
                #    (should generate error messages to stderr)

# Bad behavior for random input : -1 point each for each not handling properly
                #
                # for these commands, just pound on the keyboard and hit
                # <ENTER> at random times and see if the program behaves
                # properly by not crashing, you don't have to look at the
                # output data
                #
                # if the program seg faults, please apply the seg fault rule
                # above
                #
                ./proj1 hexdump
                ./proj1 enc-base64
                ./proj1 dec-base64
