#include "Calendar.hpp"

void Calendar::pushHoliday(const Date& date)
{
	if (m_size == 0)
	{
		m_size++;
		m_days = new Day[m_size];
		m_days[0].setDate(date);
		m_days[0].setIsWeekend(true);
		return;
	}
	Day* newDays = new Day[m_size + 1];
	for (size_t i = 0; i < m_size; i++)
	{
		newDays[i] = m_days[i];
	}
	newDays[m_size].setDate(date);
	newDays[m_size].setIsWeekend(true);
	m_size++;
	if (m_days != nullptr)
	{
		delete m_days;
	}
	m_days = newDays;
}

void Calendar::pushDay(const Day& day)
{
	if (m_size == 0)
	{
		m_size++;
		m_days = new Day[m_size];
		m_days[0] = day;
		return;
	}
	Day* newDays = new Day[m_size + 1];
	for (size_t i = 0; i < m_size; i++)
	{
		newDays[i] = m_days[i];
	}
	newDays[m_size] = day;
	m_size++;
	if (m_days != nullptr)
	{
		delete m_days;
	}
	m_days = newDays;
}

void Calendar::pushMeeting(const Date& date, const Meeting& meeting)
{
	size_t dayPos = findDay(date);
	if (dayPos == 0xffffffff)
	{
		pushDay(Day(date, meeting));
	}
	else
	{
		m_days[dayPos].pushMeeting(meeting);
	}

}

void Calendar::removeDay(size_t position)
{
	if (m_size == 1)
	{
		m_size = 0;
		delete[] m_days;
		m_days = nullptr;
		return;
	}
	Day* newDays = new Day[m_size - 1];
	for (size_t i = 0; i < position; i++)
	{
		newDays[i] = m_days[i];
	}
	for (size_t i = position + 1; i < m_size; i++)
	{
		newDays[i - 1] = m_days[i];
	}
	m_size--;
	delete[] m_days;
	m_days = newDays;
}

size_t Calendar::findDay(const Date& date)
{
	for (size_t i = 0; i < m_size; i++)
	{
		if (m_days[i].getDate() == date)
		{
			return i;
		}
	}
	return 0xffffffff;
}

size_t Calendar::findMeeting(size_t dayPos, size_t startTime, size_t endTime)
{
	size_t meetingsSize = m_days->getMeetingSize();
	for (size_t j = 0; j < meetingsSize ; j++)
	{
		if (m_days[dayPos].getMeeting(j).getStartTime() == startTime && m_days[dayPos].getMeeting(j).getEndTime() == endTime)
		{
			return j;
		}
	}
	return 0xffffffff;
}

size_t Calendar::findMeeting(size_t dayPos, size_t startTime)
{
	size_t meetingsSize = m_days->getMeetingSize();
	for (size_t j = 0; j < meetingsSize; j++)
	{
		if (m_days[dayPos].getMeeting(j).getStartTime() == startTime)
		{
			return j;
		}
	}
	return 0xffffffff;
}

size_t Calendar::meetingLength(const Meeting& meeting)
{
	size_t length = meeting.getEndTime() - meeting.getStartTime();

	if (meeting.getEndTime() % 100 < meeting.getStartTime() % 100)
	{
		length -= 40;
	}
	return length;
}

void Calendar::swapDays(Day& first, Day& second)
{
	Day temp(first);
	first = second;
	second = temp;
}

void Calendar::sortDays()
{
	for (size_t i = 0; i < m_size - 1; i++)
	{
		for (size_t j = 0; j < m_size - i - 1; j++)
		{
			if (m_days[j].getDate().getYear() > m_days[j + 1].getDate().getYear())
			{
				swapDays(m_days[j], m_days[j + 1]);
				continue;
			}
			else
			{
				if (m_days[j].getDate().getYear() == m_days[j + 1].getDate().getYear() && m_days[j].getDate().getMonth() > m_days[j + 1].getDate().getMonth())
				{
					swapDays(m_days[j], m_days[j + 1]);
					continue;
				}
				else
				{
					if (m_days[j].getDate().getMonth() == m_days[j + 1].getDate().getMonth() && m_days[j].getDate().getDay() > m_days[j + 1].getDate().getDay())
					{
						swapDays(m_days[j], m_days[j + 1]);
						continue;
					}
				}
			}
		}
	}
}

Date Calendar::cinDate()
{
	std::string dateStr;
	std::string yearStr, monthStr, dayStr;
	do
	{
		std::cout << "Enter date in format Year.Month.Day " << std::endl;
		std::cin >> dateStr;
		while(!ValidateDateFormat(dateStr))
		{
			std::cout << "Invalid format. " << std::endl;
			std::cout << "Enter date in format Year.Month.Day " << std::endl;
			std::cin >> dateStr;
		}
		size_t firstDot = dateStr.find('.');
		size_t secondDot = dateStr.rfind('.');
		yearStr = dateStr.substr(0, firstDot);
		monthStr = dateStr.substr(firstDot + 1, secondDot - firstDot - 1);
		dayStr = dateStr.substr(secondDot + 1);

	} while (!ValidateDate(yearStr, monthStr, dayStr));
	size_t year, month, day;
	year = std::stoul(yearStr);
	month = std::stoul(monthStr);
	day = std::stoul(dayStr);
	return Date(year, month, day);
}

bool Calendar::ValidateDateFormat(const std::string& dateStr)
{
	size_t firstDot = dateStr.find('.');
	size_t secondDot = dateStr.rfind('.');
	if (firstDot == std::string::npos)
	{
		return false;
	}
	if (secondDot == firstDot + 1)
	{
		return false;
	}
	if (firstDot == 0)
	{
		return false;
	}
	if (secondDot == dateStr.length() - 1)
	{
		return false;
	}
	if (firstDot == secondDot)
	{
		return false;
	}
	if (dateStr.find('.', firstDot + 1) != secondDot)
	{
		return false;
	}
	return true;
}

bool Calendar::ValidateDate(const std::string& yearStr, const std::string& monthStr, const std::string& dayStr)
{
	if (yearStr.find_first_not_of("0123456789") != std::string::npos)
	{
		std::cout << "Invalid Year. " << std::endl;
		return false;
	}
	if (monthStr.find_first_not_of("0123456789") != std::string::npos)
	{
		std::cout << "Invalid Month. " << std::endl;
		return false;
	}
	if (dayStr.find_first_not_of("0123456789") != std::string::npos)
	{
		std::cout << "Invalid day. " << std::endl;
		return false;
	}
	size_t year = std::stoul(yearStr), month = std::stoul(monthStr), day = std::stoul(dayStr);
	if (month < 1 || month > 12)
	{
		std::cout << "Invalid Month. " << std::endl;
		return false;
	}
	if (Date(year,month,day).isLeapYear())
	{
		if (month == 2)
		{
			if (DAYS_IN_MONTH[1] + 1 < day)
			{
				std::cout << "Invalid Day. " << std::endl;
				return false;
			}
			else
			{
				return true;
			}
		}
		else
		{
			if (DAYS_IN_MONTH[month - 1] < day)
			{
				std::cout << "Invalid Day. " << std::endl;
				return false;
			}
			else
			{
				return true;
			}
		}
	}
	else
	{
		if (DAYS_IN_MONTH[month - 1] < day)
		{
			std::cout << "Invalid Day. " << std::endl;
			return false;
		}
		else
		{
			return true;
		}
	}
	return true;
}

size_t Calendar::cinTime(const char* startOrEnd)
{
	std::string timeStr;
	std::string hoursStr, minutesStr;
	do
	{
		std::cout << "Enter " << startOrEnd << "time in format Hours:Minutes " << std::endl;
		std::cin >> timeStr;
		while (!ValidateTimeFormat(timeStr))
		{
			std::cout << "Invalid format. " << std::endl;
			std::cout << "Enter " << startOrEnd << "time in format Hours:Minutes " << std::endl;
			std::cin >> timeStr;
		}
		size_t colum = timeStr.find(':');
		hoursStr = timeStr.substr(0, colum);
		minutesStr = timeStr.substr(colum + 1);

	} while (!ValidateTime(hoursStr, minutesStr));
	size_t time = 0;
	time = std::stoul(minutesStr);
	time += std::stoul(hoursStr)*100;
	return time;
}

bool Calendar::ValidateTimeFormat(const std::string& timeStr)
{
	size_t colum = timeStr.find(':');
	size_t otherColum = timeStr.rfind(':');
	if (colum == std::string::npos)
	{
		return false;
	}
	if (otherColum != colum)
	{
		return false;
	}
	if (colum == 0)
	{
		return false;
	}
	if (colum == timeStr.length() - 1)
	{
		return 0;
	}
	return true;

}

bool Calendar::ValidateTime(const std::string& hoursStr, const std::string& minutesStr)
{
	if (hoursStr.find_first_not_of("0123456789") != std::string::npos)
	{
		std::cout << "Invalid Hours. " << std::endl;
		return false;
	}
	if (minutesStr.find_first_not_of("0123456789") != std::string::npos)
	{
		std::cout << "Invalid Minutes. " << std::endl;
		return false;
	}
	size_t hours = std::stoul(hoursStr), minutes = std::stoul(minutesStr);
	if (hours > 24)
	{
		std::cout << "Invalid Hours. " << std::endl;
		return false;
	}
	if (minutes > 59)
	{
		std::cout << "Invalid Minutes. " << std::endl;
		return false;
	}
	return true;

}

Calendar::Calendar(const char* fileName) :
	m_days(nullptr),
	m_size(0)
{
	std::ofstream outStream;
	outStream.open(fileName);
	outStream.close();
	std::ifstream inStream;
	inStream.open(fileName);
	if (inStream.peek() == EOF)
	{
		std::cout << "Empty" << std::endl;// do nothing.
	}
	else
	{
		std::cout << "Not empty" << std::endl;// load calendar from file
	}
	inStream.close();
}

Calendar::~Calendar()
{
	for (size_t i = 0; i < m_size; i++)
	{
		delete[] &(m_days[i].getMeeting(0));
	}
	delete[] m_days;
}

void Calendar::book()
{
	//Date
	Date date(cinDate());
	//Meeting
	size_t startTime = cinTime("start"), endTime = cinTime("end");
	std::string name, note;

	std::cout << "Enter name: ";
	std::cin >> name;
	std::cin.ignore();
	std::cout << "Enter note: ";
	std::getline(std::cin, note);

	size_t dayPos = findDay(date);
	if (dayPos == 0xffffffff)
	{
		pushDay(Day(date, Meeting(startTime, endTime, name, note)));
	}
	else
	{
		if (m_days[dayPos].getIsWeekend())
		{
			std::cout << "This is a holiday." << std::endl;
			return;
		}
		else
		{
			m_days[dayPos].pushMeeting(Meeting(startTime, endTime, name, note));
		}
	}
	
}

void Calendar::unbook()
{
	//Date
	Date date(cinDate());
	//startTime, endTime
	size_t startTime = cinTime("start"), endTime = cinTime("end");

	size_t dayPos = findDay(date);
	if (dayPos == 0xffffffff)
	{
		std::cout << "Day not found." << std::endl;
		return;
	}
	else
	{
		if (m_days[dayPos].getIsWeekend())
		{
			std::cout << "This is a holiday." << std::endl;
			return;

		}
		else
		{
			size_t meetingPos = findMeeting(dayPos, startTime, endTime);
			if (meetingPos == 0xffffffff)
			{
				std::cout << "Meeting not found." << std::endl;
				return;
			}
			else
			{
				m_days[dayPos].removeMeeting(meetingPos);
				if (m_days[dayPos].getMeetingSize() == 0)
				{
					removeDay(dayPos);
				}
			}
		}
	}

}

void Calendar::agenda()
{
	//Date
	Date date(cinDate());

	size_t dayPos = findDay(date);
	if (dayPos == 0xffffffff)
	{
		std::cout << "Day not found." << std::endl;
		return;
	}
	else
	{
		if (m_days[dayPos].getIsWeekend())
		{
			std::cout << "This is a holiday." << std::endl;
			return;
		}
		else
		{
			m_days[dayPos].sortMeetings();
			size_t meetingSize = m_days[dayPos].getMeetingSize();
			for (size_t i = 0; i < meetingSize; i++)
			{
				std::cout << m_days[dayPos].getMeeting(i);
			}
		}
	}
}

void Calendar::change()
{
	//Date
	Date date(cinDate());
	//startTime
	size_t startTime = cinTime("start");
	//option
	size_t option;

	size_t dayPos = findDay(date);
	size_t meetingPos;
	if (dayPos == 0xffffffff)
	{
		std::cout << "Day not found." << std::endl;
		return;
	}
	else
	{
		if (m_days[dayPos].getIsWeekend())
		{
			std::cout << "This is a holiday." << std::endl;
			return;
		}
		else
		{
			meetingPos = findMeeting(dayPos, startTime);
			if (meetingPos == 0xffffffff)
			{
				std::cout << "Meeting not found." << std::endl;
				return;
			}
		}
	}

	do
	{
		std::cout << "Choose what to change. " << std::endl;
		std::cout << "(1) Date." << std::endl;
		std::cout << "(2) Start time." << std::endl;
		std::cout << "(3) End time." << std::endl;
		std::cout << "(4) Name." << std::endl;
		std::cout << "(5) Note." << std::endl;
		std::cout << "Your choice: " << std::endl;
		std::cin >> option;

	} while (option < 1 || option>5);

	//newValue
	Meeting savedMeeting = m_days[dayPos].getMeeting(meetingPos);
	std::string name;

	size_t duration = 0;//meeting duration
	switch (option)
	{
	case 1:
		date = cinDate();
		m_days[dayPos].removeMeeting(meetingPos);
		pushMeeting(date, savedMeeting);
		break;
	case 2:
		startTime = cinTime("new start");
		m_days[dayPos].removeMeeting(meetingPos);
		duration = meetingLength(savedMeeting);
		savedMeeting.setStartTime(startTime);
		if (startTime >= savedMeeting.getEndTime())
		{
			if ((savedMeeting.getStartTime() % 100 + duration % 100) > 59)
			{
				savedMeeting.setEndTime(startTime + duration + 40);
			}
			else
			{
				savedMeeting.setEndTime(startTime + duration);
			}
		}
		m_days[dayPos].pushMeeting(savedMeeting);
		break;
	case 3:
		startTime = cinTime("new end");
		m_days[dayPos].removeMeeting(meetingPos);
		savedMeeting.setEndTime(startTime);
		m_days[dayPos].pushMeeting(savedMeeting);
		break;
	case 4:
		std::cout << "Enter new name: ";
		std::cin >> name;
		m_days[dayPos].removeMeeting(meetingPos);
		savedMeeting.setName(name);
		m_days[dayPos].pushMeeting(savedMeeting);
		break;
	case 5:
		std::cout << "Enter new note: ";
		std::cin.ignore();
		std::getline(std::cin, name);
		m_days[dayPos].removeMeeting(meetingPos);
		savedMeeting.setNote(name);
		m_days[dayPos].pushMeeting(savedMeeting);
		break;
	}
}

void Calendar::find()
{
	//string
	std::string subStr;
	std::cout << "Enter word to search for: ";
	std::cin >> subStr;

	size_t meetingSize;
	for (size_t i = 0; i < m_size; i++)
	{
		if (m_days[i].getIsWeekend())
		{
			continue;
		}
		else
		{
			meetingSize = m_days[i].getMeetingSize();
			for (size_t j = 0; j < meetingSize; j++)
			{
				if (m_days[i].getMeeting(j).getName().find(subStr) != std::string::npos || m_days[i].getMeeting(j).getNote().find(subStr) != std::string::npos)
				{
					std::cout << "Date: " << m_days[i].getDate() << std::endl;
					std::cout << m_days[i].getMeeting(j);
				}
			}
		}
	}
}

void Calendar::holiday()
{
	//Date
	Date date(cinDate());

	size_t dayPos = findDay(date);
	if (dayPos == 0xffffffff)
	{
		pushHoliday(date);
	}
	else
	{
		m_days[dayPos].setIsWeekend(true);
	}
	
}

void Calendar::busyDays()
{
	//From
	std::cout << "From when to start the search?" << std::endl;
	Date fromDate(cinDate());
	//to
	std::cout << "When to stop the search?" << std::endl;
	Date toDate(cinDate());

	size_t* busyHours = new size_t[m_size]{ 0 };

	for (size_t i = 0; i < m_size; i++)
	{
		if (m_days[i].getIsWeekend())
		{
			continue;
		}
		else
		{
			if ((m_days[i].getDate() >= fromDate)
				&& (m_days[i].getDate() <= toDate))
			{
				size_t meetingSize = m_days[i].getMeetingSize();
				for (size_t j = 0; j < meetingSize; j++)
				{
					busyHours[i] += meetingLength(m_days[i].getMeeting(j));
				}
			}
		}
	}

	for (size_t i = 0; i < m_size - 1; i++)
	{
		for (size_t j = 0; j < m_size - i - 1; j++)
		{
			if (busyHours[j] < busyHours[j + 1])
			{
				size_t temp = busyHours[j];
				busyHours[j] = busyHours[j + 1];
				busyHours[j + 1] = temp;

				swapDays(m_days[j], m_days[j + 1]);
			}
		}
	}

	for (size_t i = 0; i < m_size; i++)
	{
		if (busyHours[i] == 0)
		{
			break;
		}
		else
		{
			std::cout << "Date: " << m_days[i].getDate() << std::endl;
			size_t meetingSize = m_days[i].getMeetingSize();
			for (size_t j = 0; j < meetingSize; j++)
			{
				std::cout << m_days[i].getMeeting(j);
			}
		}
	}
	delete[] busyHours;
}

void Calendar::findSlot()
{
	//Date
	Date date(cinDate());
	//hours
	size_t meetingLength = cinTime("meeting");
	std::string name, note;

	std::cout << "Enter name: ";
	std::cin >> name;
	std::cin.ignore();
	std::cout << "Enter note: ";
	std::getline(std::cin, note);


	for (size_t i = 0; i < m_size; i++)
	{
		if (m_days[i].getIsWeekend())
		{
			continue;
		}
		if (m_days[i].getDate() >= date)
		{
			m_days[i].sortMeetings();
			size_t meetingSize = m_days[i].getMeetingSize();
			if (meetingSize > 0)
			{
				if (800 + meetingLength <= m_days[i].getMeeting(0).getStartTime())
				{
					m_days[i].pushMeeting(Meeting(800, 800 + meetingLength, name, note));
					return;
				}
			}
			for (size_t j = 0; j < meetingSize - 1; j++)
			{
				if (m_days[i].getMeeting(j).getEndTime() <= 1700)
				{
					if (m_days[i].getMeeting(j).getEndTime() + meetingLength <= m_days[i].getMeeting(j + 1).getStartTime())
					{
						m_days[i].pushMeeting(Meeting(m_days[i].getMeeting(j).getEndTime(), m_days[i].getMeeting(j).getEndTime() + meetingLength, name, note));
						return;
					}
				}
			}
		}
	}
	sortDays();
	date = m_days[m_size - 1].getDate();
	date.nextDay();
	pushDay(Day(date, Meeting(800, 800 + meetingLength, name, note)));
}

void Calendar::print()//temporary for testing
{
	for (size_t i = 0; i < m_size; i++)
	{
		std::cout << "Date: " << m_days[i].getDate().getYear() << "." << m_days[i].getDate().getMonth() << "." << m_days[i].getDate().getDay() << std::endl;
		if (m_days[i].getIsWeekend())
		{
			std::cout << "Holiday!" << std::endl;
			continue;
		}
		size_t meetingSize = m_days[i].getMeetingSize();
		for (size_t j = 0; j < meetingSize; j++)
		{
			std::cout << m_days[i].getMeeting(j);
		}
	}
}
