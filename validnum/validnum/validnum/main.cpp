
#include <iostream>
#include <string>
#include <vector>
#include <set>
using namespace std;
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
        vector<char> emptyvec;
        transitions.swap(emptyvec);
        accepts.clear();
    }

    void AddTrans(int i, int j, char c)
    {
        int index = i * this->statenum + j;
        if (index >= 0 && index < transitions.size())
        {
            transitions[index] = c;
        }
    }

    int GetTrans(int i, char c)
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
        s = Strip(s);
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
            while (i < s.size() && j >= 0 && i < j)
            {
                bool hasblank = false;
                if (s.at(i) == ' ' && i + 1 < s.size())
                {
                    ++i;
                    hasblank = true;
                }

                if (s.at(j) == ' ' && j - 1 >= 0)
                {
                    --j;
                    hasblank = true;
                }

                if (!hasblank)
                {
                    break;;
                }
            }
            ret = s.substr(istart, iend - istart);
        }

        return ret;
    }
    int statenum;
    vector<char> transitions;
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
        static DFA normalnumber(20);
        static  DFA realnumber(10);
        static  DFA scinumber(10);
        if (!initialized)
        {
            for (int i = 1; i <= 9; ++i)
            {
                normalnumber.AddTrans(0, i, '0' + i);
                normalnumber.MarkAccept(i);
            }
            
            for (int i = 1; i <= 19; ++i)
            {
                normalnumber.MarkAccept(i);
                for (int j = 10; j <= 19; ++j)
                {
                    normalnumber.AddTrans(i, j, '0' + j - 10);
                }
            }

            initialized = true;
        }

        if (normalnumber.Run(s) || realnumber.Run(s) || scinumber.Run(s))
        {
            return true;
        }
        return false;
    }
};



int main()
{
    Solution s;
    string sstr = "0123456";
    bool isnum = s.isNumber(sstr);
    cout << sstr << "  isNum? " << isnum << endl;
    getchar();
    return 0;
}