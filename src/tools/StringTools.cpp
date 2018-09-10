#include "StringTools.h"

StringTools::StringTools()
{
    unicodeTable.insert("0430", "a");
    unicodeTable.insert("0431", "б");
    unicodeTable.insert("0432", "в");
    unicodeTable.insert("0433", "г");
    unicodeTable.insert("0434", "д");
    unicodeTable.insert("0435", "е");
    unicodeTable.insert("0451", "ё");
    unicodeTable.insert("0436", "ж");
    unicodeTable.insert("0437", "з");
    unicodeTable.insert("0438", "и");
    unicodeTable.insert("0439", "й");
    unicodeTable.insert("043A", "к");
    unicodeTable.insert("043B", "л");
    unicodeTable.insert("043C", "м");
    unicodeTable.insert("043D", "н");
    unicodeTable.insert("043E", "о");
    unicodeTable.insert("043F", "п");
    unicodeTable.insert("0440", "р");
    unicodeTable.insert("0441", "с");
    unicodeTable.insert("0442", "т");
    unicodeTable.insert("0443", "у");
    unicodeTable.insert("0444", "ф");
    unicodeTable.insert("0445", "х");
    unicodeTable.insert("0446", "ц");
    unicodeTable.insert("0447", "ч");
    unicodeTable.insert("0448", "ш");
    unicodeTable.insert("0449", "щ");
    unicodeTable.insert("044A", "ъ");
    unicodeTable.insert("044B", "ы");
    unicodeTable.insert("044C", "ь");
    unicodeTable.insert("044D", "э");
    unicodeTable.insert("044E", "ю");
    unicodeTable.insert("044F", "я");

    unicodeTable.insert("0410", "A");
    unicodeTable.insert("0411", "Б");
    unicodeTable.insert("0412", "В");
    unicodeTable.insert("0413", "Г");
    unicodeTable.insert("0414", "Д");
    unicodeTable.insert("0415", "Е");
    unicodeTable.insert("0401", "Ё");
    unicodeTable.insert("0416", "Ж");
    unicodeTable.insert("0417", "З");
    unicodeTable.insert("0418", "И");
    unicodeTable.insert("0419", "Й");
    unicodeTable.insert("041A", "К");
    unicodeTable.insert("041B", "Л");
    unicodeTable.insert("041C", "М");
    unicodeTable.insert("041D", "Н");
    unicodeTable.insert("041E", "О");
    unicodeTable.insert("041F", "П");
    unicodeTable.insert("0420", "Р");
    unicodeTable.insert("0421", "С");
    unicodeTable.insert("0422", "Т");
    unicodeTable.insert("0423", "У");
    unicodeTable.insert("0424", "Ф");
    unicodeTable.insert("0425", "Х");
    unicodeTable.insert("0426", "Ц");
    unicodeTable.insert("0427", "Ч");
    unicodeTable.insert("0428", "Ш");
    unicodeTable.insert("0429", "Щ");
    unicodeTable.insert("042A", "Ъ");
    unicodeTable.insert("042B", "Ы");
    unicodeTable.insert("042C", "Ь");
    unicodeTable.insert("042D", "Э");
    unicodeTable.insert("042E", "Ю");
    unicodeTable.insert("042F", "Я");
}

QChar StringTools::toCyrConverter(const QString& unicode)
{
    return unicodeTable[unicode.toUpper()][0];
}

QString StringTools::convertUnicodeToCyrillic(const QByteArray& unicodeBytes)
{
    QString str = QString::fromUtf8(unicodeBytes);
    int idx = -1;
    while ((idx = str.indexOf("\\u") ) != -1)
    {
          QString  uc = toCyrConverter(str.mid(idx + 2, 4));
          str.replace(idx, 6, uc);
    }
    return str;
}

bool StringTools::convertPhoneNumber(QString& phone)
{
    if (phone.length() > 12)
    {
        return false;
    }
    else if(phone.length() == 12)  //+7 ok
    {
        if( phone.at(0) == '+' && phone.at(1) == '7')
        {

        }
        else
        {
            return false;
        }
    }
    else if (phone.length() == 11) //8 or 7
    {
        const QChar firstChar = phone.at(0);
        if(firstChar == '7')
        {
            phone = '+' + phone;
        }
        else if (firstChar == '8')
        {
            phone = '+' + phone;
            phone[1] = '7';
        }
        else
        {
            return false;
        }
    }
    else if (phone.length() == 10)//  906 660 67 90
    {
        phone = "+7" + phone;
    }
    else
    {
        return false;
    }

    return true;
}
