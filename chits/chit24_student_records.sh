#!/bin/bash
#
# Problem Statement (Chit 24):
# Implement a program (shell script) that allows a user to manage student records by taking Student Roll Number and Student Name as input. The program should perform basic CRUD operations (Create, Read, Update, Delete) and store all records in a file for persistent storage.
#

###############################################################################
# Repeated Chit: Student Record Management
# -----------------------------------------------------------------------------
# This problem statement is the same as chit 01.
# Run the same implementation so behavior stays identical.
###############################################################################

DIR="$(cd "$(dirname "$0")" && pwd)"
exec "$DIR/chit01_student_records.sh"

