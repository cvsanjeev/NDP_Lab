#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 4444
#define MAX_STUDENTS 10

// Student details structure
struct Student {
    int regNumber;
    char name[256];
    char residentialAddress[256];
    char dept[256];
    int semester;
    char section;
    char coursesRegistered[256];
    int marksObtained;
    char subjectCode[256];
};

// Array of 10 hardcoded student details
struct Student studentArray[MAX_STUDENTS] = {
    {1001, "John", "123 Main St", "CSE", 3, 'A', "OS, DBMS", 85, "CS101"},
    {1002, "Alice", "456 Elm St", "ECE", 4, 'B', "Digital Electronics", 92, "EC201"},
    {1003, "Bob", "789 Oak St", "MECH", 2, 'C', "Thermodynamics", 78, "ME102"},
    {1004, "Eve", "101 Pine St", "CIVIL", 3, 'A', "Structural Analysis", 88, "CE202"},
    {1005, "Charlie", "202 Maple St", "IT", 4, 'B', "Web Development", 90, "IT301"},
    {1006, "Grace", "303 Cedar St", "CSE", 3, 'A', "Algorithms, Data Structures", 87, "CS202"},
    {1007, "David", "404 Birch St", "ECE", 2, 'C', "Digital Circuits", 76, "EC102"},
    {1008, "Emily", "505 Spruce St", "MECH", 4, 'B', "Fluid Mechanics", 91, "ME301"},
    {1009, "Frank", "606 Willow St", "CIVIL", 3, 'A', "Construction Management", 84, "CE202"},
    {1010, "Helen", "707 Redwood St", "IT", 2, 'C', "Database Management", 79, "IT102"}
};

// Function to handle registration number request
void handleRegistrationNumber(int client_socket, int regNumber) {
	char response[512];
	printf("%d",regNumber);
    for (int i = 0; i < MAX_STUDENTS; i++) {
        if (studentArray[i].regNumber == regNumber) {
            // Send student's details and PID to the client

            sprintf(response, "Name: %s\nResidential Address: %s\nDepartment: %s\nSemester: %d\nSection: %c\nCourses Registered: %s\nMarks Obtained: %d\nSubject Code: %s\nPID: %d",
                    studentArray[i].name, studentArray[i].residentialAddress, studentArray[i].dept, studentArray[i].semester, studentArray[i].section,
                    studentArray[i].coursesRegistered, studentArray[i].marksObtained, studentArray[i].subjectCode, getpid());

            send(client_socket, response, sizeof(response), 0);
            return;
        }
    }
    sprintf(response, "Registration Number %d not found\nPID: %d", regNumber, getpid());
    send(client_socket, response, sizeof(response), 0);
}

// Function to handle student name request
void handleStudentName(int client_socket, const char *studentName) {
	char response[1024];
    for (int i = 0; i < MAX_STUDENTS; i++) {
        if (strcmp(studentArray[i].name, studentName) == 0) {
            // Send student's details and PID to the client

            sprintf(response, "Registration Number: %d\nResidential Address: %s\nDepartment: %s\nSemester: %d\nSection: %c\nCourses Registered: %s\nMarks Obtained: %d\nSubject Code: %s\nPID: %d",
                    studentArray[i].regNumber, studentArray[i].residentialAddress, studentArray[i].dept, studentArray[i].semester, studentArray[i].section,
                    studentArray[i].coursesRegistered, studentArray[i].marksObtained, studentArray[i].subjectCode, getpid());

            send(client_socket, response, sizeof(response), 0);
            return;
        }
    }

    sprintf(response, "Student Name %s not found\nPID: %d", studentName, getpid());
    send(client_socket, response, sizeof(response), 0);
}

// Function to handle subject code request
void handleSubjectCode(int client_socket, const char *subjectCode) {

	char response[512];
    for (int i = 0; i < MAX_STUDENTS; i++) {
        if (strcmp(studentArray[i].subjectCode, subjectCode) == 0) {
            // Send student's details and PID to the client
            sprintf(response, "Registration Number: %d\nName: %s\nResidential Address: %s\nDepartment: %s\nSemester: %d\nSection: %c\nCourses Registered: %s\nMarks Obtained: %d\nPID: %d",
                    studentArray[i].regNumber, studentArray[i].name, studentArray[i].residentialAddress, studentArray[i].dept, studentArray[i].semester, studentArray[i].section,
                    studentArray[i].coursesRegistered, studentArray[i].marksObtained, getpid());

            send(client_socket, response, sizeof(response), 0);
            return;
        }
    }

    // If the subject code is not found
    sprintf(response, "Subject Code %s not found\nPID: %d", subjectCode, getpid());
    send(client_socket, response, sizeof(response), 0);
}


//MAIN FUNCTION
int main() {
    int server_fd, new_socket;
    struct sockaddr_in server_addr, new_addr;
    socklen_t addr_size;
    pid_t child_pid;

    // Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        printf("Error in socket");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = PORT;
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind
    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        printf("Error in binding");
        exit(1);
    }

    // Listen
    if (listen(server_fd, 10) == 0) {
        printf("Listening...\n");
    } else {
        printf("Error in listening\n");
    }
    addr_size = sizeof(new_addr);



	//MAINPART STARTS HERE

    while (1) {
        new_socket = accept(server_fd, (struct sockaddr*)&new_addr, &addr_size); // Accept connection
        if (new_socket < 0) {
            printf("Error in accepting");
            exit(1);
        }

        // Fork a child process to handle the client request
        if (fork() == 0) {
            close(server_fd); // Close the listening socket in the child

            int option;
            recv(new_socket, &option, sizeof(int), 0);

            // Handle the request based on the received option
            switch (option) {
                case 1: {
                    int regNumber;
                    recv(new_socket, &regNumber, sizeof(int), 0);
                    handleRegistrationNumber(new_socket, regNumber);
                    break;
                }
                case 2: {
                    char studentName[256];
                    recv(new_socket, studentName, sizeof(studentName), 0);
                    handleStudentName(new_socket, studentName);
                    break;
                }
                case 3: {
                    char subjectCode[256];
                    recv(new_socket, subjectCode, sizeof(subjectCode), 0);
                    handleSubjectCode(new_socket, subjectCode);
                    break;
                }
                default:
                    printf("Invalid option received\n");
            }

            close(new_socket);
            exit(0);
        } else {
            // Parent process
            close(new_socket); // Close the new socket in the parent
        }
    }

    return 0;
}
