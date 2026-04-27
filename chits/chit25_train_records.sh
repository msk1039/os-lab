#!/bin/bash
#
# Problem Statement (Chit 25):
# Implement a program (shell script) that allows a user to manage train records by taking train Number and train Name as input. The program should perform basic CRUD operations (Create, Read, Update, Delete) and store all records in a file for persistent storage.
#

###############################################################################
# Train Record Management using Bash
# -----------------------------------------------------------------------------
# Stores records in a text file as: train_number|train_name
#
# Example:
# 1. Create train=12101, name="Express" -> stores: 12101|Express
# 2. Read                              -> prints all saved train records
# 3. Update train=12101 to "Superfast" -> changes name
# 4. Delete train=12101                -> removes record
###############################################################################

FILE="trains.txt"
touch "$FILE"

create_record() {
    read -p "Enter train number: " number
    read -p "Enter train name: " name

    if grep -q "^$number|" "$FILE"; then
        echo "Record already exists."
    else
        echo "$number|$name" >> "$FILE"
        echo "Record added."
    fi
}

read_records() {
    if [ ! -s "$FILE" ]; then
        echo "No records found."
        return
    fi

    echo "Train Number | Train Name"
    echo "-------------------------"
    while IFS="|" read -r number name; do
        echo "$number | $name"
    done < "$FILE"
}

update_record() {
    read -p "Enter train number to update: " number

    if grep -q "^$number|" "$FILE"; then
        read -p "Enter new train name: " name
        grep -v "^$number|" "$FILE" > temp.txt
        echo "$number|$name" >> temp.txt
        mv temp.txt "$FILE"
        echo "Record updated."
    else
        echo "Record not found."
    fi
}

delete_record() {
    read -p "Enter train number to delete: " number

    if grep -q "^$number|" "$FILE"; then
        grep -v "^$number|" "$FILE" > temp.txt
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

