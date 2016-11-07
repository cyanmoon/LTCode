#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <unordered_set>
#include <set>
#include <stack>
using namespace std;

class Solution {
public:
    inline unsigned getDis(unsigned i, unsigned j, unsigned sizel)
    {
        unsigned index = i * sizel + j;
        return m_edges[index];
    }

    inline void setDis(unsigned i, unsigned j, unsigned val, unsigned sizel)
    {
        unsigned index = i * sizel + j;
        m_edges[index] = val;
        index = j * sizel + i;
        m_edges[index] = val;
    }

    vector<vector<string>> findLadders(string beginWord, string endWord, unordered_set<string> &wordList) 
    {
        
        auto listSize = wordList.size();
        m_edges.resize(listSize * listSize);
        vector<set<unsigned> > formerIds;
        formerIds.resize(listSize);
        unsigned int index = 0;
        for (auto i = wordList.cbegin(); i != wordList.cend(); ++i)
        {
            m_id2str.insert(pair<unsigned, string>(index, *i));
            m_str2id.insert(pair<string, unsigned>(*i, index));
            m_words.push_back(*i);
            m_ids.push_back(index);
            cout << index << " -> " << *i << endl;
            ++index;
        }

        for (auto i = 0; i < listSize; ++i)
        {
            for (auto j = 0; j < listSize; ++j)
            {
                if (i == j)
                {
                    continue;
                }

                if (canTransTo(m_words[i], m_words[j]))
                {
                    setDis(i, j, 1, listSize);
                    formerIds[i].insert(j);
                    formerIds[j].insert(i);
                }
            }
        }

        vector<string> v;
        vector<vector <string> > vl;
        auto startIndex = m_str2id[beginWord];
        auto endIndex = m_str2id[endWord];
        if (startIndex == endIndex)
        {
            vector<string> v;
            v.push_back(beginWord);
            vector<vector <string> > vl;
            vl.push_back(v);
            return vl;
        }

        bool changed = false;
        while (true)
        {
            changed = false;
            for (auto i = 0; i < listSize; ++i)
            {
                if (i == startIndex)
                {
                    continue;
                }

                for (auto j = 0; j < listSize; ++j)
                {
                    if (j == startIndex || i == j)
                    {
                        continue;
                    }

                    auto dissj = getDis(startIndex, j, listSize);
                    auto dissi = getDis(startIndex, i, listSize);
                    auto disij = getDis(i, j, listSize);
                    if (dissj == 0)
                    {
                        if (dissi != 0 && disij != 0)
                        {
                            setDis(startIndex, j, dissi + disij, listSize);
                            formerIds[j].insert(i);
                            changed = true;
                        }
                    }
                    else
                    {
                        if (dissi != 0 && disij != 0 && disij + dissi < dissj)
                        {
                            setDis(startIndex, j, dissi + disij, listSize);
                            formerIds[j].clear();
                            formerIds[j].insert(i);
                            changed = true;
                        }
                        else if (dissi != 0 && disij != 0 && disij + dissi == dissj && formerIds[j].find(i) == formerIds[j].cend())
                        {
                            formerIds[j].insert(i);
                            changed = true;
                        }
                    }
                }
            }

            if (!changed)
            {
                break;
            }
        }

        cout << "."<< endl;
        return vl;
    }

    bool canTransTo(string beginWord, string endWord)
    {
        if (beginWord.length() != endWord.length())
        {
            return false;
        }

        int diffnum = 0;
        for (auto i = 0; i < beginWord.size(); ++i)
        {
            if (beginWord[i] != endWord[i])
            {
                ++diffnum;
                if (diffnum > 1)
                {
                    return false;
                }
            }
        }

        return diffnum == 1;
    }

private:
    vector<unsigned> m_edges;
    map<unsigned, string> m_id2str;
    map<string, unsigned> m_str2id;
    vector<string> m_words;
    vector<unsigned> m_ids;
};

void Test(string from, string to, vector<string> testsample, Solution& solution)
{
    unordered_set<string> s;
    for (auto i = testsample.begin(); i != testsample.end(); ++i)
    {
        s.insert(*i);
    }

    solution.findLadders(from, to, s);
}

int main(int argc, char **argv)
{
    Solution solution;
    vector<string> s;
    s.push_back("hit");
    s.push_back("hot");
    s.push_back("lot");
    s.push_back("lit");
    s.push_back("lis");
    s.push_back("kis");
    Test(string("hit"), string("kis"), s, solution);
    getchar();
    return 0;
}
