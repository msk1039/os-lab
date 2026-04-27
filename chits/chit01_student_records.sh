#!/bin/bash
#
# Problem Statement (Chit 1):
# Implement a program (shell script) that allows a user to manage student records by taking Student Roll Number and Student Name as input. The program should perform basic CRUD operations (Create, Read, Update, Delete) and store all records in a file for persistent storage.
#

###############################################################################
# Student Record Management using Bash
# -----------------------------------------------------------------------------
# Stores records in a text file as: roll_number|student_name
#
# Example:
# 1. Create roll=101, name="Amit"  -> stores: 101|Amit
# 2. Read                         -> prints all saved records
# 3. Update roll=101 to "Neha"    -> changes line to: 101|Neha
# 4. Delete roll=101              -> removes that line
###############################################################################

FILE="students.txt"
touch "$FILE"

create_record() {
    read -p "Enter roll number: " roll
    read -p "Enter student name: " name

    if grep -q "^$roll|" "$FILE"; then
        echo "Record already exists."
    else
        echo "$roll|$name" >> "$FILE"
        echo "Record added."
    fi
}

read_records() {
    if [ ! -s "$FILE" ]; then
        echo "No records found."
        return
    fi

    echo "Roll Number | Student Name"
    echo "--------------------------"
    while IFS="|" read -r roll name; do
        echo "$roll | $name"
    done < "$FILE"
}

update_record() {
    read -p "Enter roll number to update: " roll

    if grep -q "^$roll|" "$FILE"; then
        read -p "Enter new student name: " name
        grep -v "^$roll|" "$FILE" > temp.txt
        echo "$roll|$name" >> temp.txt
        mv temp.txt "$FILE"
        echo "Record updated."
    else
        echo "Record not found."
    fi
}

delete_record() {
    read -p "Enter roll number to delete: " roll

    if grep -q "^$roll|" "$FILE"; then
        grep -v "^$roll|" "$FILE" > temp.txt
        mv temp.txt "$FILE"
        echo "Record deleted."
    else
        echo "Record not found."
    fi
}

while true; do
    echo
    echo "1. Create"
    echo "2. Read"
    echo "3. Update"
    echo "4. Delete"
    echo "5. Exit"
    read -p "Enter choice: " choice

    case "$choice" in
        1) create_record ;;
        2) read_records ;;
        3) update_record ;;
        4) delete_record ;;
        5) exit 0 ;;
        *) echo "Invalid choice." ;;
    esac
done

