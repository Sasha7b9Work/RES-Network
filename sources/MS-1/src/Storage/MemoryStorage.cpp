// 2023/09/08 22:14:01 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Storage/MemoryStorage.h"
#include "Utils/Math.h"
#include "Hardware/Timer.h"
#include <cstring>
#include <cstdlib>


namespace MemoryStorage
{
    static const uint BEGIN = 0;
    static const uint END = 1024 * 1024;

    static const int NUM_PAGES = (END - BEGIN) / W25Q80DV::SIZE_PAGE;

    struct Page
    {
        void Init(int num_page)
        {
            startAddress = BEGIN + W25Q80DV::SIZE_PAGE * (uint)num_page;
        }

        Record FirstRecord()
        {
            return Record(startAddress);
        }

        int GetMaxRecordsCount() const
        {
            return W25Q80DV::SIZE_PAGE / sizeof(Record);
        }

        Record LastRecord()
        {
            return Record(startAddress + (uint)GetMaxRecordsCount() * sizeof(Record));
        }

        int GetRecordsCount()
        {
            int result = 0;

            for (Record record = FirstRecord(); record < LastRecord(); record++)
            {
                if (record.IsValid())
                {
                    result++;
                }
            }

            return result;
        }

        void Prepare()
        {
            for (Record record = FirstRecord(); record < LastRecord(); record++)
            {
                if (record.IsEmpty() || record.IsErased())
                {
                    continue;
                }

                if (!record.IsValid())
                {
                    record.Erase();
                }
            }
        }

        // ���������� ������ ��������� ��� ������ �������� ��� nullptr, ���� ��� ������
        static Page *GetFirstForRecord();

        // ���������� �������� � ����� ������ �������
        static Page *GetWithOldestRecord();

        bool GetFirstEmptyRecord(Record *out)
        {
            for (Record record = FirstRecord(); record < LastRecord(); record++)
            {
                if (record.IsEmpty())
                {
                    *out = record;

                    return true;
                }
            }

            return false;
        }

        Record Append(Measurements &measurements);

        void Erase()
        {
            int num_page = (int)(startAddress / W25Q80DV::SIZE_PAGE);

            W25Q80DV::ErasePage(num_page);
        }

        bool IsEmpty()
        {
            for (Record record = FirstRecord(); record < LastRecord(); record++)
            {
                if (record.IsEmpty())
                {
                    continue;
                }

                return false;
            }

            return true;
        }

        int GetLastNumber()
        {
            int result = 0;

            for (Record record = FirstRecord(); record < LastRecord(); record++)
            {
                if (record.IsValid() && record.GetNumber() > result)
                {
                    result = record.GetNumber();
                }
            }

            return result;
        }

        bool GetOldestRecord(Record *out)
        {
            Record result(0);

            bool exist = false;     // ���� true, �� � result ��� ���-�� ��������

            for (Record record = FirstRecord(); record < LastRecord(); record++)
            {
                if (record.IsValid())
                {
                    if (!exist || (record.GetNumber() < result.GetNumber()))
                    {
                        exist = true;

                        result = record;
                    }
                }
            }

            if (exist)
            {
                *out = result;
            }

            return exist;
        }

        // ���������� ������ � ����������� ������������ num_record �������
        bool GetMinRecord(int num_record, Record *out)
        {
            Record result(0);

            bool exist = false;     // ���� true, �� � result ��� ���-�� ��������

            for (Record record = FirstRecord(); record < LastRecord(); record++)
            {
                if (record.IsValid())
                {
                    if (exist)
                    {
                        if (record.GetNumber() >= num_record && record.GetNumber() < result.GetNumber())
                        {
                            result = record;
                        }
                    }
                    else
                    {
                        if (record.GetNumber() >= num_record)
                        {
                            exist = true;

                            result = record;
                        }
                    }
                }
            }

            if (exist)
            {
                *out = result;
            }

            return exist;
        }

        bool GetNewestRecord(Record *out)
        {
            Record result(0);

            bool exist = false;     // ���� true, �� � result ��� ���-�� ��������

            for (Record record = FirstRecord(); record < LastRecord(); record++)
            {
                if (record.IsValid())
                {
                    if (!exist || (record.GetNumber() > result.GetNumber()))
                    {
                        result = record;
                    }
                }
            }

            if (exist)
            {
                *out = result;
            }

            return exist;
        }

    private:

        uint startAddress;
    };

    static Page pages[NUM_PAGES];

    static int GetNewNumber()
    {
        int result = 0;

        for (int i = 0; i < NUM_PAGES; i++)
        {
            Page &page = pages[i];

            if (page.IsEmpty())
            {
                continue;
            }

            if (page.GetLastNumber() > result)
            {
                result = page.GetLastNumber();
            }
        }

        return result + 1;
    }

    Page *Page::GetWithOldestRecord()
    {
        Page *result = nullptr;

        for (int i = 0; i < NUM_PAGES; i++)
        {
            Page &page = pages[i];

            if (page.IsEmpty())
            {
                continue;
            }

            int last_number = page.GetLastNumber();

            if (last_number != 0)
            {
                if (!result || (last_number < result->GetLastNumber()))
                {
                    result = &page;
                }
            }
        }

        return result;
    }

    Page *Page::GetFirstForRecord()
    {
        for (int i = 0; i < NUM_PAGES; i++)
        {
            Record record(0);

            if (pages[i].GetFirstEmptyRecord(&record))
            {
                return &pages[i];
            }
        }

        return nullptr;
    }

    Record Page::Append(Measurements &measurements)
    {
        Record record(0);

        if (!GetFirstEmptyRecord(&record))
        {
            Erase();

            record = FirstRecord();
        }

        record.Write(GetNewNumber(), measurements);

        return record;
    }

    // ��������� ��� ������� �� ������� ����������� ������� � ������� ��
    static void Prepare();

    int GetRecordsCount();
}


void MemoryStorage::Init()
{
    for (int i = 0; i < NUM_PAGES; i++)
    {
        pages[i].Init(i);
    }

    Prepare();
}


void MemoryStorage::Erase(Record *record)
{
    record->Erase();
}


void MemoryStorage::Append(Measurements &meas)
{
    Page *page = Page::GetFirstForRecord();

    if (!page)
    {
        page = Page::GetWithOldestRecord();
    }

    page->Append(meas);
}


bool MemoryStorage::GetOldest(Record *record)
{
    return Record::Oldest(record);
}


void MemoryStorage::Prepare()
{
    for (int i = 0; i < NUM_PAGES; i++)
    {
        pages[i].Prepare();
    }
}


int MemoryStorage::GetRecordsCount()
{
    int result = 0;

    for (int i = 0; i < NUM_PAGES; i++)
    {
        result += pages[i].GetRecordsCount();
    }

    return result;
}


bool Record::Oldest(Record *record)
{
    bool exist_result = false;

    for (int i = 0; i < MemoryStorage::NUM_PAGES; i++)
    {
        Record oldest(0);

        if (MemoryStorage::pages[i].GetOldestRecord(&oldest))
        {
            if (!exist_result || oldest.GetMeasurements().number < record->GetMeasurements().number)
            {
                exist_result = true;

                *record = oldest;
            }
        }
    }

    return exist_result;
}


bool Record::Newest(Record *record)
{
    bool exist_result = false;

    for (int i = 0; i < MemoryStorage::NUM_PAGES; i++)
    {
        Record newest(0);

        if (MemoryStorage::pages[i].GetNewestRecord(&newest))
        {
            if (!exist_result || newest.GetMeasurements().number > record->GetMeasurements().number)
            {
                exist_result = true;

                *record = newest;
            }
        }
    }

    return exist_result;
}


#include "MemoryStorage_test.inc"
