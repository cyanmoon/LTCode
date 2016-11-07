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
        vector<set<unsigned> > nextIds;
        nextIds.resize(listSize);
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
                    nextIds[i].insert(j);
                    nextIds[j].insert(i);
                }
            }
        }

        vector<string> v;
        vector<vector <string> > vl;
        auto startIndex = m_str2id[beginWord];
        auto endIndex = m_str2id[endWord];
        if (startIndex == endIndex)
        {
            v.push_back(beginWord);
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
                            nextIds[i].insert(j);
                            changed = true;
                        }
                    }
                    else
                    {
                        if (dissi != 0 && disij != 0 && disij + dissi < dissj)
                        {
                            setDis(startIndex, j, dissi + disij, listSize);
                            nextIds[i].clear();
                            nextIds[i].insert(j);
                            changed = true;
                        }
                        else if (dissi != 0 && disij != 0 && disij + dissi == dissj && nextIds[i].find(j) == nextIds[i].cend())
                        {
                            nextIds[i].insert(j);
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
        map<unsigned, unsigned> processed;
        processed[startIndex] = 1;
        auto paths = getPathsTo(startIndex, endIndex, processed, nextIds);
        for(auto vecItor = paths.cbegin(); vecItor != paths.cend(); ++vecItor)
        {
            vector<string> v;
            for(auto idItor = vecItor->cbegin(); idItor != vecItor->cend(); ++idItor)
            {
                v.push_back(m_words[*idItor]);
            }
            vl.push_back(v);
        }
        return vl;
    }
    
    vector<vector<unsigned> > getPathsTo(unsigned ufrom, unsigned uto, map<unsigned, unsigned> processed, vector<set<unsigned> >& nextIds)
    {
        vector<unsigned> v;
        vector<vector<unsigned> > vv;
        if(ufrom == uto)
        {
            v.push_back(ufrom);
            vv.push_back(v);
            return vv;
        }
        
        set<unsigned> connectTo = nextIds[ufrom];
        for(auto i = connectTo.cbegin(); i != connectTo.cend(); ++i)
        {
            if(processed.find(*i) == processed.cend())
            {
                processed[*i] = 1;
                if(uto == *i)
                {
                    v.push_back(ufrom);
                    v.push_back(uto);
                    vv.push_back(v);
                }
                else
                {
                    auto l = getPathsTo(*i, uto,  processed, nextIds);
                    if(l.size() > 0)
                    {
                        for(auto vecItor = l.cbegin(); vecItor != l.cend(); ++vecItor)
                        {
                            vector<unsigned> vec;
                            vec.push_back(ufrom);
                            vec.insert(vec.cend(), vecItor->cbegin(), vecItor->cend());
                            vv.push_back(vec);
                        }
                    }

                }
            }
            else
            {
                ++processed[*i];
            }
        }
        
        return vv;
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

    auto paths = solution.findLadders(from, to, s);
    cout<<"Answer " << from << " --> " << to << ":" << endl;
    for(auto vecItor = paths.cbegin(); vecItor != paths.cend(); ++vecItor)
    {
        for(auto strItor = vecItor->cbegin(); strItor != vecItor->cend(); ++strItor)
        {
            cout << *strItor << " -> ";
        }
        cout<< "$" << endl;
    }
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
