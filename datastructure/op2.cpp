#include <iostream>
#include <fstream>
#include <unordered_map>
#include <cstring>
#include <string>
#include <cstdlib>
using namespace std;

struct Node
{
    string song;
    string filepath;
    Node *next;
    Node *prev;
};

Node *recentlyPlayed = nullptr;
unordered_map<string, string> songMap;

void tofile(const string &song, const string &filepath)
{
    fstream f1;
    f1.open("playlist.txt", ios::out | ios::app);
    f1 << song << "," << filepath << endl;
    f1.close();
}

void printlist(Node *first)
{
    if (first == nullptr)
    {
        cout << "The playlist is empty." << endl;
        return;
    }

    cout << "Playlist:\n";
    Node *temp = first;
    while (temp != nullptr)
    {
        cout << "Song: " << temp->song << " | File Path: " << temp->filepath << endl;
        temp = temp->next;
    }
}

void add_node(Node *&first)
{
    string songName;
    string songPath;

    cout << "\nEnter Song name: ";
    cin.ignore();
    getline(cin, songName);
    cout << "Enter Song File Path: ";
    getline(cin, songPath);

    songMap[songName] = songPath;

    Node *newNode = new Node();
    newNode->song = songName;
    newNode->filepath = songPath;
    newNode->next = nullptr;
    newNode->prev = nullptr;

    if (first == nullptr)
    {
        first = newNode;
    }
    else
    {
        Node *temp = first;
        while (temp->next != nullptr)
        {
            temp = temp->next;
        }
        temp->next = newNode;
        newNode->prev = temp;
    }

    tofile(songName, songPath);
}

void playSong(Node *&currentSong)
{
    if (currentSong == nullptr)
    {
        cout << "\nNo song is selected to play." << endl;
        return;
    }

    Node *recentNode = new Node();
    recentNode->song = currentSong->song;
    recentNode->filepath = currentSong->filepath;
    recentNode->next = recentlyPlayed;
    recentNode->prev = nullptr;

    if (recentlyPlayed != nullptr)
    {
        recentlyPlayed->prev = recentNode;
    }
    recentlyPlayed = recentNode;

    while (true)
    {
        cout << "\n=> Now Playing: " << currentSong->song << endl;
        system(("start " + currentSong->filepath).c_str());

        int choice;
        cout << "\n1. Pause (not supported by system call)\n2. Next Song\n3. Previous Song\n4. Stop and Go Back to Main Menu\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 2:
            if (currentSong->next != nullptr)
            {
                currentSong = currentSong->next;
                cout << "Skipping to the next song..." << endl;
            }
            else
            {
                cout << "This is the last song in the playlist." << endl;
            }
            break;
        case 3:
            if (currentSong->prev != nullptr)
            {
                currentSong = currentSong->prev;
                cout << "Going back to the previous song..." << endl;
            }
            else
            {
                cout << "This is the first song in the playlist." << endl;
            }
            break;
        case 4:
            cout << "Stopping playback and returning to main menu..." << endl;
            return;
        default:
            cout << "Invalid choice. Please try again." << endl;
        }
    }
}

void play(Node *first)
{
    string songName;
    cout << "\nEnter the song you wish to play: ";
    cin.ignore();
    getline(cin, songName);

    if (songMap.find(songName) != songMap.end())
    {
        Node *temp = first;
        while (temp != nullptr)
        {
            if (temp->song == songName)
            {
                playSong(temp);
                break;
            }
            temp = temp->next;
        }
    }
    else
    {
        cout << "\n# Song not found in the playlist." << endl;
    }
}

void display_recently_played()
{
    Node *current = recentlyPlayed;
    if (current == nullptr)
    {
        cout << "NO recently played tracks.\n";
        return;
    }
    cout << "Recently played tracks:\n";
    while (current != nullptr)
    {
        cout << "Song: " << current->song << " | File Path: " << current->filepath << endl;
        current = current->next;
    }
}

void free_memory(Node *first)
{
    Node *temp;
    while (first != nullptr)
    {
        temp = first;
        first = first->next;
        delete temp;
    }
}

void free_recently_played()
{
    Node *temp;
    while (recentlyPlayed != nullptr)
    {
        temp = recentlyPlayed;
        recentlyPlayed = recentlyPlayed->next;
        delete temp;
    }
}

int main()
{
    Node *first = nullptr;
    int choice;

    do
    {
        cout << "\n1. Add Song\n2. Play Song\n3. Display Playlist\n4. Show Recently Played Songs\n5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice)
        {
        case 1:
            add_node(first);
            break;
        case 2:
            play(first);
            break;
        case 3:
            printlist(first);
            break;
        case 4:
            display_recently_played();
            break;
        case 5:
            cout << "Exiting..." << endl;
            break;
        default:
            cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 5);

    free_memory(first);
    free_recently_played();

    return 0;
}
