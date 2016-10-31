
#include <iostream>
#include <string>
#include <vector>
#include <set>
using namespace std;
enum EnumChaType
{
    EnumChaType_Invalid = 129,
    EnumChaType_Number,
    EnumChaType_Number_Except0,
};

class DFA
{
public:
    DFA(int num)
    {
        this->statenum = num;
        transitions.resize(this->statenum*this->statenum);
        for (auto i = 0; i < transitions.size(); ++i)
        {
            transitions[i] = INVALID_CHAR;
        }
    }

    ~DFA()
    {
        vector<int> emptyvec;
        transitions.swap(emptyvec);
        accepts.clear();
    }

    void AddTrans(int i, int j, int c)
    {
        int index = i * this->statenum + j;
        if (index >= 0 && index < transitions.size())
        {
            transitions[index] = c;
        }
    }

    int Convert(int c)
    {
        if (c >= '0' && c <= '9')
        {
            return EnumChaType_Number;
        }

        return c;
    }

    int GetTrans(int i, int c)
    {
        int startindex = i * statenum;
        for (int i = 0; i < statenum; ++i)
        {
            int index = startindex + i;
            if (transitions[index] == c)
            {
                return i;
            }
        }
        return INVALID_STATE;
    }

    void MarkAccept(int i)
    {
        accepts.insert(i);
    }

    bool Run(string s)
    {
        if (s.empty())
        {
            return false;
        }

        int curstate = 0;
        for (auto i = 0; i < s.size(); ++i)
        {
            char c = s.at(i);
            auto tmpstate = GetTrans(curstate, c);
            if (tmpstate == INVALID_STATE)
            {
                tmpstate = GetTrans(curstate, Convert(c));
            }
            if (tmpstate == INVALID_STATE)
            {
                return false;
            }

            curstate = tmpstate;
        }

        bool finded = accepts.find(curstate) != accepts.cend();
        return finded;
    }

    string Strip(string s)
    {
        string ret = "";
        if (s.size() > 0)
        {
            int istart = 0;
            int iend = s.size();
            auto i = 0;
            auto j = s.size() - 1;
            while (i < s.size() && j >= 0 && i <= j)
            {
                bool hasblank = false;
                if (s.at(i) == ' ')
                {
                    ++i;
                    hasblank = true;
                }

                if (s.at(j) == ' ')
                {
                    --j;
                    hasblank = true;
                }

                istart = i;
                iend = j;
                if (!hasblank)
                {
                    break;;
                }
            }
            ret = s.substr(istart, iend - istart + 1);
        }

        return ret;
    }
    int statenum;
    vector<int> transitions;
    set<int> accepts;
    const int INVALID_STATE = -1;
    const char INVALID_CHAR = -128;
};


class Solution 
{
public:
    bool isNumber(string s) 
    {
        static bool initialized = false;
        static DFA normalnumber(3);
        static DFA negnormalnumber(21);
/*        static DFA realnumber(8);*/
        static DFA scinumber(8);
        static DFA scirealnumber(11);
        s = scinumber.Strip(s);
        if (!initialized)
        {
            //normal number dfa
            normalnumber.AddTrans(0, 1, '-');
            normalnumber.AddTrans(1, 2, EnumChaType_Number);
            normalnumber.AddTrans(0, 2, EnumChaType_Number);
            normalnumber.AddTrans(2, 2, EnumChaType_Number);
            normalnumber.MarkAccept(2);

            //real number dfa
//             realnumber.AddTrans(0, 1, EnumChaType_Number);
//             realnumber.AddTrans(1, 2, EnumChaType_Number);
//             realnumber.AddTrans(2, 2, EnumChaType_Number);
//             realnumber.AddTrans(3, 4, EnumChaType_Number);
//             realnumber.AddTrans(4, 5, EnumChaType_Number);
//             realnumber.AddTrans(5, 5, EnumChaType_Number);
//             realnumber.AddTrans(1, 3, '.');
//             realnumber.AddTrans(2, 3, '.');
//             realnumber.AddTrans(0, 6, '-');
//             realnumber.AddTrans(3, 7, '-');
//             realnumber.AddTrans(6, 1, EnumChaType_Number);
//             realnumber.AddTrans(7, 4, EnumChaType_Number);
//             realnumber.MarkAccept(4);
//             realnumber.MarkAccept(5);

            //sci number dfa
            scinumber.AddTrans(0, 1, EnumChaType_Number);
            scinumber.AddTrans(1, 2, EnumChaType_Number);
            scinumber.AddTrans(2, 2, EnumChaType_Number);
            scinumber.AddTrans(3, 4, EnumChaType_Number);
            scinumber.AddTrans(4, 5, EnumChaType_Number);
            scinumber.AddTrans(5, 5, EnumChaType_Number);
            scinumber.AddTrans(1, 3, 'e');
            scinumber.AddTrans(2, 3, 'e');
            scinumber.AddTrans(0, 6, '-');
            scinumber.AddTrans(3, 7, '-');
            scinumber.AddTrans(6, 1, EnumChaType_Number);
            scinumber.AddTrans(7, 4, EnumChaType_Number);
            scinumber.MarkAccept(4);
            scinumber.MarkAccept(5);


            //sci real number dfa
            scirealnumber.AddTrans(0, 1, EnumChaType_Number);
            scirealnumber.AddTrans(1, 2, EnumChaType_Number);
            scirealnumber.AddTrans(2, 2, EnumChaType_Number);
            scirealnumber.AddTrans(3, 4, EnumChaType_Number);
            scirealnumber.AddTrans(4, 5, EnumChaType_Number);
            scirealnumber.AddTrans(5, 5, EnumChaType_Number);
            scirealnumber.AddTrans(0, 3, '.');
            scirealnumber.AddTrans(1, 3, '.');
            scirealnumber.AddTrans(2, 3, '.');
            scirealnumber.AddTrans(6, 3, '.');
            scirealnumber.AddTrans(0, 6, '-');
            scirealnumber.AddTrans(3, 7, '-');
            scirealnumber.AddTrans(6, 1, EnumChaType_Number);
            scirealnumber.AddTrans(7, 4, EnumChaType_Number);
            scirealnumber.AddTrans(4, 8, 'e');
            scirealnumber.AddTrans(5, 8, 'e');
            scirealnumber.AddTrans(3, 8, 'e');
            scirealnumber.AddTrans(8, 9, '-');
            scirealnumber.AddTrans(9, 10, EnumChaType_Number);
            scirealnumber.AddTrans(8, 10, EnumChaType_Number);
            scirealnumber.AddTrans(10, 10, EnumChaType_Number);
            scirealnumber.MarkAccept(10);
            scirealnumber.MarkAccept(4);
            scirealnumber.MarkAccept(5);
            scirealnumber.MarkAccept(3);
/*            scirealnumber.MarkAccept(5);*/

            initialized = true;
        }

        bool isnormalnumber = normalnumber.Run(s);
        //bool isrealnumber = realnumber.Run(s);
        bool isscinumber = scinumber.Run(s);
        bool isscirealnumber = scirealnumber.Run(s);
        if (isnormalnumber || isscinumber || (isscirealnumber && !(s.size() == 1 && s[0] == '.')))
        {
            return true;
        }
        return false;
    }
};



int main()
{
    /* Test Examples
    "-.3"
    ".3"
    "3."
    "-3."
    "."
    " ."
    ". "
    ".e1"
    ""
    */
    Solution s;
    string sstr = " -15";
    bool isnum = s.isNumber(sstr);
    cout << sstr << "  isNum? " << isnum << endl;

    sstr = "-1.5045654754";
    isnum = s.isNumber(sstr);
    cout << sstr << "  isNum? " << isnum << endl;

    sstr = "10e-5045654754";
    isnum = s.isNumber(sstr);
    cout << sstr << "  isNum? " << isnum << endl;

    sstr = "10.0e-5045654754";
    isnum = s.isNumber(sstr);
    cout << sstr << "  isNum? " << isnum << endl;
    
    sstr = "-10.0e-5045654754";
    isnum = s.isNumber(sstr);
    cout << sstr << "  isNum? " << isnum << endl;

    sstr = "-10.0e-5045654754.0";
    isnum = s.isNumber(sstr);
    cout << sstr << "  isNum? " << isnum << endl;

    DFA a(1);
    string oris = " 123 ";
    oris = "   .";
    cout << "oris:_" << oris << "_stripped:_" << a.Strip(oris) << "_" << endl;
    getchar();
    return 0;
}