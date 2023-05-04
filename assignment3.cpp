#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

size_t hashValue(string value)
{
    return std::hash<std::string>()(value);
}

class Node
{
private:
    int version;
    size_t hash;
    string content;
    Node *next;

public:
    Node(int version, size_t hash, string content, Node *next = nullptr)
    {
        this->version = version;
        this->hash = hash;
        this->content = content;

        this->next = next;
    }
    size_t getHash() { return this->hash; }
    int getVersion() { return this->version; }
    string getContent() { return this->content; }
    Node *getNext() { return this->next; }
    void *setNext(Node *next)
    {
        this->next = next;
    }
};

class LinkedList
{
private:
    Node *head;

public:
    LinkedList()
    {
        this->head = nullptr;
    }

    void push(int version, size_t hash, string content)
    {
        if (this->head == nullptr)
        {
            this->head = new Node(version, hash, content);
        }
        else
        {
            Node *last = this->head;
            while (last->getNext() != nullptr)
            {
                last = last->getNext();
            }
            last->setNext(new Node(version, hash, content));
        }
    }

    int pop()
    {
        size_t hashValue = this->head->getHash();
        Node *tmp = this->head;
        this->head = this->head->getNext();
        delete tmp;
        return hashValue;
    }

    int length()
    {
        int cnt = 0;
        Node *curr = this->head;
        while (curr != nullptr)
        {
            cnt++;
            curr = curr->getNext();
        }
        return cnt;
    }

    Node *gethead()
    {
        return this->head;
    }

    bool find(int version)
    {
        Node *curr = this->head;
        while (curr != nullptr)
        {
            if ((curr->getVersion()) == version)
                return true;
            curr = curr->getNext();
        }
        return false;
    }
};

class Git322
{

public:
    void add(void);
    void print(void);
    void callLoad(void);
    void callRemove(void);
    void callCompare(void);
    void callSearch(void);

    Git322();

protected:
    int version = 0;
    void load(int version);
    void remove(int version);
    void search(string keyword);
    void compare(int version1, int version2);
    LinkedList mylist;
};

Git322::Git322(void) {}

void Git322::add()
{
    ifstream infile;
    string newContent;
    infile.open("file.txt");

    getline(infile, newContent, '^');
    size_t newHash = std::hash<std::string>()(newContent);

    Node *curr = mylist.gethead();
    while (curr != nullptr)
    {

        if (newHash == (curr->getHash()))
        {
            cout << "git322 did not detect any change to your file and will not create a new version.\n"
                 << endl;
            return;
        };
        curr = curr->getNext();
    };

    mylist.push(++version, newHash, newContent);

    infile.close();

    cout << "Your content has been added successfully.\n"
         << endl;
    return;
};

void Git322::print(void)
{
    cout << "Number of versions: " + to_string(mylist.length()) + "\n"
         << endl;

    Node *curr = mylist.gethead();
    while (curr != nullptr)
    {
        cout << "Version number: " + to_string(curr->getVersion()) + "\n"
             << endl;
        cout << "Hash value: " + to_string(curr->getHash()) + "\n"
             << endl;
        cout << "Content: " + curr->getContent() + "\n\n"
             << endl;
        curr = curr->getNext();
    };

    return;
};

void Git322::load(int version)
{
    std::remove("File.txt");
    ofstream newFile("File.txt");

    Node *curr = mylist.gethead();
    while (curr != nullptr)
    {
        if ((curr->getVersion()) == version)
        {

            newFile << curr->getContent();
            newFile.close();
            return;
        }
        curr = curr->getNext();
    };
    return;
}
void Git322::callLoad(void)
{
    bool validVersion = false;
    int versionNum;

    cout << "Which version would you like to load?\n"
         << endl;
    while (!validVersion)
    {

        if (cin >> versionNum)
        {
            if (mylist.find(versionNum))
            {
                load(versionNum);
                validVersion = true;
            }
            else
            {
                cout << "Please enter a valid version number. If you are not sure please press 'p' to list all valid version numbers.\n"
                     << endl;
                continue;
            }
        }
        else
        {
            cin.clear();           // clear error state
            if (cin.peek() == 'p') // see what was in the instream to have caused the error
            {
                break; // return to options list
            }
            cin.ignore(256, '\n'); // go to the end of the stream so we can try recieveing more data

            cout << "Please enter a valid version number. If you are not sure please press 'p' to list all valid version numbers.\n"
                 << endl;
            continue;
        }
    }

    cout << "Version " + to_string(versionNum) + " loaded successfully. Please refresh your text editor to see the changes.\n"
         << endl;
    return;
}

void Git322::compare(int version1, int version2) //"Empty Line" means a line w/o char; "End Of File" means sstream eof
{

    string content1;
    string content2;

    Node *curr = mylist.gethead();
    while (curr != nullptr)
    {
        if (curr->getVersion() == version1)
        {
            content1 = curr->getContent();
        }

        else if (curr->getVersion() == version2)
        {
            content2 = curr->getContent();
        }
        curr = curr->getNext();
    };

    std::istringstream ss1(content1);
    std::istringstream ss2(content2);

    cout << content1;
    cout << content2;

    std::string line1;
    std::string line2;

    int i = 1; // line index
    string emptyLine = "<Empty Line>";
    while (!(ss1.peek() < 0) && !(ss2.peek() < 0)) // peek first in case we're at stream eof; ascii values are postiive, sstream eof is usually -1
    {

        getline(ss1, line1, '\n');
        getline(ss2, line2, '\n');

        if (hashValue(line1) == hashValue(line2))
        {
            cout << "Line " + to_string(i) + " : <Identical>\n"
                 << endl;
        }
        else
        {

            // not identical
            if (line1.empty())
            {
                line1 = emptyLine;
            }
            if (line2.empty()) // line without any characters
            {
                line2 = emptyLine;
            }

            cout << "Line " + to_string(i) + " : " + line1 + " <<>> " + line2 + "\n"
                 << endl;
        }

        i++;
    }

    while (getline(ss1, line1, '\n')) // if ss1 has more lines than ss2
    {
        if (line1.empty())
        {
            cout << "Line " + to_string(i) + " : <Empty Line> <<>> <End of File> \n"
                 << endl;
        }
        else
        {
            cout << "Line " + to_string(i) + " : " + line1 + " <<>> <End of File> \n"
                 << endl;
        }
        i++;
    };

    while (getline(ss2, line2, '\n')) // if ss2 has more lines than ss1
    {
        if (line2.empty())
        {
            cout << "Line " + to_string(i) + " : <End of File> <<>> <Empty Line> \n"
                 << endl;
        }
        else
        {
            cout << "Line " + to_string(i) + " : <End of File> <<>> " + line2 + "\n"
                 << endl;
        }
        i++;
    }
    return;
}

void Git322::callCompare(void)
{
    int versionNum1, versionNum2;

    cout << "Please enter the number of the first version to compare: ";
    if (cin >> versionNum1)

    {
        if (mylist.find(versionNum1))
        {
            cout << "Please enter the number of the second version to compare: ";
            if (cin >> versionNum2)
            {
                if (mylist.find(versionNum2))
                {
                    compare(versionNum1, versionNum2);
                    return;
                }
            }

            cin.clear();           // clear error state
            cin.ignore(256, '\n'); // go to the end of the stream so we can try recieveing more data

            cout << "Invalid version number two, compare exited.\n";
            return;
        }
    }
    cin.clear();           // clear error state
    cin.ignore(256, '\n'); // go to the end of the stream so we can try recieveing more data

    cout << "Invalid version number one, enter 'c' to compare another two files.\n";

    return;
}

void Git322::search(string keyword)
{
    bool found = false;
    Node *curr = mylist.gethead();
    string result = "";

    for (int i = 0; i < mylist.length(); i++)
    {
        string content = curr->getContent();
        if (content.find(keyword) != std::string::npos) // find also returns true for substrings
        {
            found = true;
            result += "\n";
            result += ("Version number: " + to_string(curr->getVersion()) + "\n");
            result += ("Hash value: " + to_string(curr->getHash()) + "\n");
            result += ("Content: " + curr->getContent() + "\n\n");
        }
        curr = curr->getNext();
    }

    if (!found)
    {
        cout << "Your keyword '" + keyword + "' was not found in any version.\n"
             << endl;
        return;
    }

    cout << "The keyword '" + keyword + "' has been found in the following versions: " << endl;
    cout << result << endl;

    return;
}

void Git322::callSearch()
{
    string keyword;

    cout << "Please enter the keyword that you are looking for: " << endl;
    if (cin >> keyword)
    {
        search(keyword);
    }

    else
    {
        cout << "Invalid Input.\n"
             << endl;
    }
    return;
}

void Git322::remove(int version)
{

    Node *curr = mylist.gethead();
    if (curr == nullptr)
    {
        cout << "No versions to delete; add before deleting." << endl;
        return;
    }
    if ((curr->getVersion()) == version)
    {
        mylist.pop();
        return;
    }

    Node *prev;

    for (int i = 1; i < mylist.length(); i++)
    {
        prev = curr;
        curr = curr->getNext();
        if ((curr->getVersion()) == version)
        {
            Node *tmp = curr;
            prev->setNext(curr->getNext());
            delete tmp;
            return;
        }
    }

    return;
}

void Git322::callRemove()
{
    bool validVersion = false;
    int versionNum;

    cout << "Enter the number of the version that you want to delete: " << endl;
    while (!validVersion)
    {

        if (cin >> versionNum)
        {
            if (mylist.find(versionNum))
            {
                remove(versionNum);
                validVersion = true;
            }
            else
            {
                cout << "Please enter a valid version number.\n"
                     << endl;
                continue;
            }
        }
        else
        {
            cin.clear();           // clear error state
            cin.ignore(256, '\n'); // go to the end of the stream so we can try recieveing more data

            cout << "Please enter a valid version number.\n"
                 << endl;
            continue;
        }
    }

    cout << "Version " + to_string(versionNum) + " deleted successfully.\n"
         << endl;
    return;
}

class EnhancedGit322 : public Git322
{

protected:
    vector<string> fileNames;

public:
    ~EnhancedGit322()
    {
        if (mylist.length() == 0)
        {
            return;
        }

        ofstream newFile("PersistedFile.txt");

        Node *curr = mylist.gethead();

        while (curr != nullptr)
        {
            newFile << (to_string(curr->getVersion()) + ";" + curr->getContent() + "^");
            curr = curr->getNext();
        };

        newFile.close();
        return;
    }
    EnhancedGit322()
    {
        ifstream infile;
        std::string line;
        int latest_ver;

        infile.open("PersistedFile.txt");

        while (getline(infile, line, '^'))
        {
            stringstream ss(line);
            string ver;
            string content;

            getline(ss, ver, ';');
            getline(ss, content);

            latest_ver = stoi(ver);
            mylist.push(latest_ver, hashValue(content), content);
        }

        version = latest_ver;
        std::remove("PersistedFile.txt");
    };
};

void optionHandler(EnhancedGit322 &git)
{
    char option;

    while (true)
    {
        cin >> option;
        if (option == 'a')
        {
            git.add();
            continue;
        }

        if (option == 'p')
        {
            git.print();
            continue;
        }

        if (option == 'l')
        {
            git.callLoad();
            continue;
        }

        if (option == 'c')
        {
            git.callCompare();
            continue;
        }

        if (option == 's')
        {
            git.callSearch();
            continue;
        }

        if (option == 'r')
        {
            git.callRemove();
            continue;
        }

        if (option == 'e')
        {
            return;
        }
    }

    return;
}

int main()
{
    EnhancedGit322 *git = new EnhancedGit322();

    cout << "Welcome to the Comp322 file versioning system!\n"
         << endl;
    cout << "To add the content of your file to version control press 'a'\n"
         << endl;
    cout << "To remove a version press 'r'\n"
         << endl;
    cout << "To load a version press 'l'\n"
         << endl;
    cout << "To print to the screen the detailed list of all versions press 'p'\n"
         << endl;
    cout << "To compare any 2 versions press 'c'\n"
         << endl;
    cout << "To search versions for a keyword press 's'\n"
         << endl;
    cout << "To exit press 'e'\n"
         << endl;
    optionHandler(*git);

    delete git;

    return 0;
};