#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 44445
#define MAX_STUDENTS 10

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
void handleRegistrationNumber(int server_socket, struct sockaddr_in client_addr, int regNumber) {
	char response[512];
    socklen_t addr_size = sizeof(client_addr);
	printf("%d",regNumber);
    for (int i = 0; i < MAX_STUDENTS; i++) {
        if (studentArray[i].regNumber == regNumber) {
            // Send student's details and PID to the client

            sprintf(response, "Name: %s\nResidential Address: %s\nDepartment: %s\nSemester: %d\nSection: %c\nCourses Registered: %s\nMarks Obtained: %d\nSubject Code: %s\nPID: %d",
                    studentArray[i].name, studentArray[i].residentialAddress, studentArray[i].dept, studentArray[i].semester, studentArray[i].section,
                    studentArray[i].coursesRegistered, studentArray[i].marksObtained, studentArray[i].subjectCode, getpid());

            sendto(server_socket, response, sizeof(response), 0, (struct sockaddr*)&client_addr, addr_size);
            return;
        }
    }
    sprintf(response, "Registration Number %d not found\nPID: %d", regNumber, getpid());
    sendto(server_socket, response, sizeof(response), 0, (struct sockaddr*)&client_addr, addr_size);
}

// Function to handle student name request
void handleStudentName(int server_socket, struct sockaddr_in client_addr, const char *studentName) {
	char response[1024];
    socklen_t addr_size = sizeof(client_addr);
    for (int i = 0; i < MAX_STUDENTS; i++) {
        if (strcmp(studentArray[i].name, studentName) == 0) {
            // Send student's details and PID to the client

            sprintf(response, "Registration Number: %d\nResidential Address: %s\nDepartment: %s\nSemester: %d\nSection: %c\nCourses Registered: %s\nMarks Obtained: %d\nSubject Code: %s\nPID: %d",
                    studentArray[i].regNumber, studentArray[i].residentialAddress, studentArray[i].dept, studentArray[i].semester, studentArray[i].section,
                    studentArray[i].coursesRegistered, studentArray[i].marksObtained, studentArray[i].subjectCode, getpid());

            sendto(server_socket, response, sizeof(response), 0, (struct sockaddr*)&client_addr, addr_size);
            return;
        }
    }

    sprintf(response, "Student Name %s not found\nPID: %d", studentName, getpid());
    sendto(server_socket, response, sizeof(response), 0, (struct sockaddr*)&client_addr, addr_size);
}

// Function to handle subject code request
void handleSubjectCode(int server_socket, struct sockaddr_in client_addr, const char *subjectCode) {
    socklen_t addr_size = sizeof(client_addr);
	char response[512];
    for (int i = 0; i < MAX_STUDENTS; i++) {
        if (strcmp(studentArray[i].subjectCode, subjectCode) == 0) {
            // Send student's details and PID to the client
            sprintf(response, "Registration Number: %d\nName: %s\nResidential Address: %s\nDepartment: %s\nSemester: %d\nSection: %c\nCourses Registered: %s\nMarks Obtained: %d\nPID: %d",
                    studentArray[i].regNumber, studentArray[i].name, studentArray[i].residentialAddress, studentArray[i].dept, studentArray[i].semester, studentArray[i].section,
                    studentArray[i].coursesRegistered, studentArray[i].marksObtained, getpid());

            sendto(server_socket, response, sizeof(response), 0, (struct sockaddr*)&client_addr, addr_size);
            return;
        }
    }

    // If the subject code is not found
    sprintf(response, "Subject Code %s not found\nPID: %d", subjectCode, getpid());
    sendto(server_socket, response, sizeof(response), 0, (struct sockaddr*)&client_addr, addr_size);
}



int main() {
    int server_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size = sizeof(client_addr);

    server_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_socket < 0) {
        printf("Error in socket");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        printf("Error in binding");
        exit(1);
    }

    printf("UDP Server is running...\n");



    while (1) {
        int option;
        recvfrom(server_socket, &option, sizeof(int), 0, (struct sockaddr*)&client_addr, &addr_size);

        switch (option) {
            case 1: {
                int regNumber;
                recvfrom(server_socket, &regNumber, sizeof(int), 0, (struct sockaddr*)&client_addr, &addr_size);
                handleRegistrationNumber(server_socket, client_addr, regNumber);
                break;
            }
            // Handle other cases for UDP as needed
            default:
                printf("Invalid option received\n");
        }
    }

    return 0;
}
