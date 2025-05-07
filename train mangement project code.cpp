#include<iostream>
#include<string>
using namespace std;

struct Node {
    string data;
    Node* next;
    Node* prev;

    Node(string val) {
        data = val;
        next = NULL;
        prev = NULL;
    }
};

class Train {
public:
    int total_seats_avail_pesh = 2;
    int total_seats_avail_isb = 3;
    int total_seats_avail_lhr = 3;

    Node* pesh, *isb, *lhr;		//train nodes
    Node* pesh_w, *isb_w, *lhr_w; // Waiting list heads

    Train() {
        pesh = isb = lhr = pesh_w = isb_w = lhr_w = NULL;
    }

    // Show available seats functions
    int show_total_seats_pesh() { return total_seats_avail_pesh; }
    int show_total_seats_isb() { return total_seats_avail_isb; }
    int show_total_seats_lhr() { return total_seats_avail_lhr; }

	//insert function
    void append(string val, string city) {
        Node* &booking = (city == "Peshawar") ? pesh : (city == "Islamabad") ? isb : lhr;
        Node* &waiting = (city == "Peshawar") ? pesh_w : (city == "Islamabad") ? isb_w : lhr_w;
        int &seats = (city == "Peshawar") ? total_seats_avail_pesh : (city == "Islamabad") ? total_seats_avail_isb : total_seats_avail_lhr;

        if (seats == 0) {		//waiting list
            char choice;
            cout << "\nAll seats for " << city << " are booked.\nWould you like to be put on the waiting list? (y/n): ";
            cin >> choice;

            if (choice == 'y' || choice == 'Y') {
                Node* newN = new Node(val);
                if (waiting == NULL) {
                    waiting = newN;
                } else {
                    Node* temp = waiting;
                    while (temp->next != NULL) temp = temp->next;	//pen example
                    temp->next = newN;
                    newN->prev = temp;
                }
                cout << "Added to the waiting list (FIFO queue)!\n";
            }
            return;
        }

        Node* newN = new Node(val);
        if (booking == NULL) {
            booking = newN;
        } else {
            Node* temp = booking;
            while (temp->next != NULL) temp = temp->next;
            temp->next = newN;
            newN->prev = temp;
        }
        seats--;
        sortList(booking);		//alphabetic sort
        cout << "Booking Confirmed!\n";
    }

    void del(string val, string city) {
        Node* &booking = (city == "Peshawar") ? pesh : (city == "Islamabad") ? isb : lhr;
        Node* &waiting = (city == "Peshawar") ? pesh_w : (city == "Islamabad") ? isb_w : lhr_w;
        int &seats = (city == "Peshawar") ? total_seats_avail_pesh : (city == "Islamabad") ? total_seats_avail_isb : total_seats_avail_lhr;

        if (booking == NULL) {
            cout << "No bookings for " << city << "!\n";
            return;
        }

        Node* temp = booking;
        if (temp->data == val) {
            booking = temp->next;
            if (booking != NULL) booking->prev = NULL;
        } else {
            while (temp != NULL && temp->data != val) temp = temp->next;
            if (temp == NULL) {
                cout << "Name not found in booking list!\n";
                return;
            }
            if (temp->prev != NULL) temp->prev->next = temp->next;
            if (temp->next != NULL) temp->next->prev = temp->prev;
        }

        delete temp;

        // Promote from waiting list if available
        if (waiting != NULL) {
            string promoted = waiting->data;
            Node* toDelete = waiting;
            waiting = waiting->next;
            if (waiting != NULL) waiting->prev = NULL;
            delete toDelete;

            seats++; // temporarily increase seat count
            append(promoted, city);
            cout << "Booking cancelled. '" << promoted << "' from waiting list added to confirmed list.\n";
        } else {
            seats++;
            cout << "Booking cancelled.\n";
        }
    }

    void print(string city) {
        Node* booking = (city == "Peshawar") ? pesh : (city == "Islamabad") ? isb : lhr;
        int seats = (city == "Peshawar") ? total_seats_avail_pesh : (city == "Islamabad") ? total_seats_avail_isb : total_seats_avail_lhr;

        cout << "\nCurrent Bookings for " << city << ":\n";
        if (booking == NULL) {
            cout << "No current bookings.\n";
        } else {
            Node* temp = booking;
            while (temp != NULL) {
                cout << temp->data;
                if (temp->next != NULL) cout << " -> ";
                temp = temp->next;
            }
        }

        for (int i = 0; i < seats; i++) cout << " -> Empty";
        cout << "\n";
    }

    void sortList(Node* &head) {
        if (!head || !head->next) return;
        for (Node* i = head; i->next != NULL; i = i->next) {
            for (Node* j = i->next; j != NULL; j = j->next) {
                if (i->data > j->data) {
                    swap(i->data, j->data);
                }
            }
        }
    }
};

int main() {
    Train TT;
    int main_choice, city_choice;
    string name, city;
    char sub_choice;

    do {
        cout << "\n=== Train Booking System ===\n"
             << "1. Add Booking\n"
             << "2. Cancel Booking\n"
             << "3. Exit\n"
             << "Enter your choice: ";
        cin >> main_choice;

        switch (main_choice) {
            case 1:
            case 2:
                cout << "\nSelect Destination:\n"
                     << "1. Peshawar (Seats left: " << TT.show_total_seats_pesh() << ")\n"
                     << "2. Islamabad (Seats left: " << TT.show_total_seats_isb() << ")\n"
                     << "3. Lahore (Seats left: " << TT.show_total_seats_lhr() << ")\n"
                     << "Enter your choice: ";
                cin >> city_choice;

                if (city_choice == 1) city = "Peshawar";
                else if (city_choice == 2) city = "Islamabad";
                else if (city_choice == 3) city = "Lahore";
                else {
                    cout << "Invalid destination!\n";
                    continue;
                }

                if (main_choice == 1) {
                    cout << "Do you want to book a seat for " << city << "? (y/n): ";
                    cin >> sub_choice;
                    if (sub_choice == 'y' || sub_choice == 'Y') {
                        cout << "Enter your name: ";
                        cin >> name;
                        TT.append(name, city);
                        TT.print(city);
                    }
                } else {
                    cout << "Do you want to cancel your seat in " << city << "? (y/n): ";
                    cin >> sub_choice;
                    if (sub_choice == 'y' || sub_choice == 'Y') {
                        cout << "Enter your name: ";
                        cin.ignore();
                        getline(cin, name);
                        TT.del(name, city);
                        TT.print(city);
                    }
                }
                break;

            case 3:
                cout << "Exiting...\n";
                break;

            default:
                cout << "Invalid input!\n";
        }
    } while (main_choice != 3);

    return 0;
}