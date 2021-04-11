#include "table_mgr.hpp"
#include "console_color.hpp"

using namespace std;

TableMgr::TableMgr(vector<string> &column_names)    
{
    for (const auto &column : column_names) 
    {
        widths.push_back(column.length());
        headers.push_back(column);
    }
}


void TableMgr::setPadding(unsigned int n) 
{
    padding = n;
}


void TableMgr::setStyle(unsigned int n) 
{
    switch (n) 
    {
        case 0 :
            style = BasicStyle;
            break;
        case 1 :
            style = LineStyle;
            break;
        case 2 :
            style = DoubleLineStyle;
            break;
        case 3 :
            style = InvisibleStyle;
            break;
        default :
            style = BasicStyle;
            break;
    }
}


bool TableMgr::addRow(vector<string> &row) 
{
    if (row.size() > widths.size()) 
        throw invalid_argument{"Appended row size must be same as header size"};

    auto r = vector<string>{row};
    rows.push_back(r);
    for (unsigned int i = 0; i < r.size(); ++i) 
        widths[i] = max(r[i].size(), widths[i]);
    return true;
}


bool TableMgr::removeRow(unsigned int index) 
{
    if (index > rows.size())
        return false;

    rows.erase(rows.begin() + index);
    return true;
}


TableMgr &TableMgr::operator+=(vector<string> &row) 
{
    if (row.size() > widths.size()) 
        throw invalid_argument{"Appended row size must be same as header size"};

    addRow(row);
    return *this;
}


TableMgr &TableMgr::operator-=(const uint32_t rowIndex) 
{
    if (rows.size() < rowIndex)
        throw std::out_of_range{"Row index out of range."};

    removeRow(rowIndex);
    return *this;
}


string TableMgr::getLine(RowType rowType) const 
{
    stringstream line;
    line << rowType.left;
    for (unsigned int i = 0; i < widths.size(); ++i) 
    {
        for (unsigned int j = 0; j < (widths[i] + padding + padding); ++j) 
            line << style.horizontal;
        line << (i == widths.size() - 1 ? rowType.right : rowType.intersect);
    }
    return line.str() + "\n";
}


string TableMgr::getHeaders(Headers headers) const 
{
    stringstream line;
    line << style.vertical;
    for (unsigned int i = 0; i < headers.size(); ++i) 
    {
        string text = headers[i];
        line << SPACE_CHARACTER * padding + text + SPACE_CHARACTER * (widths[i] - text.length()) + SPACE_CHARACTER * padding;
        line << style.vertical;
    }
    line << "\n";
    return line.str();
}


string TableMgr::getRows(Rows rows) const 
{
    stringstream line;
    for (auto &row : rows) 
    {
        line << style.vertical;
        for (unsigned int j = 0; j < row.size(); ++j) 
        {
            string text = row[j];
            line << SPACE_CHARACTER * padding + text + SPACE_CHARACTER * (widths[j] - text.length()) + SPACE_CHARACTER * padding;
            line << style.vertical;
        }
        line << "\n";
    }

    return line.str();
}


ostream &operator<<(ostream &out, const TableMgr &tableMgr) 
{
    out << blue << tableMgr.getLine(tableMgr.style.top);
    out << tableMgr.getHeaders(tableMgr.headers);
    out << tableMgr.getLine(tableMgr.style.middle) << white;
    out << tableMgr.getRows(tableMgr.rows);
    out << tableMgr.getLine(tableMgr.style.bottom);
    return out;
}


bool TableMgr::sort(bool ascending) 
{
    if (ascending)
        std::sort(rows.begin(), rows.end(), less<vector<string>>());
    else
        std::sort(rows.begin(), rows.end(), greater<vector<string>>());
    return true;
}


void TableMgr::updateRow(unsigned int row, unsigned int header, string data) 
{
    if (row > rows.size() - 1)
        throw out_of_range{"Row index out of range."};
    if (header > headers.size() - 1)
        throw out_of_range{"Header index out of range."};

    rows[row][header] = data;
}


void TableMgr::updateHeader(unsigned int header, string text) 
{
    if (header > headers.size())
        throw out_of_range{"Header index out of range."};

    headers[header] = text;
}


string operator*(const string &other, int repeats) 
{
    string ret;
    ret.reserve(other.size() * repeats);
    for ( ; repeats; --repeats)
        ret.append(other);
    return ret;
}
