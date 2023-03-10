#include <string>
#include <iostream>
#include <filesystem>
typedef std::vector<std::filesystem::directory_entry> vectDirEntry;

/*

​So when we reach the last function call where there is no directory and only files,
we skip​ the async part and the consequently skip the​ for_each ​as no future object ​pushed into the vector of futures
called finalVect and ​those files ​at the next step ​will be copied into 'vect' and ​that 'vect' variable will be ​returned​ to the previous function where ​it​ is waiting on​ the line​ 'fut.get()' to get the data​ which is the vector of file names​.
This time it won't skip the for_each then it will copy the returned data into the back of 'vect' and return the whole and so forth and so on.
*/
vectDirEntry ListDirectory(std::filesystem::directory_entry&& dirPath)
{
	std::vector<std::future<std::vector<std::filesystem::directory_entry>>> finalVect;
	vectDirEntry vect;

	for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(dirPath))
	{
		if (entry.is_directory())
		{

			std::future<vectDirEntry> fut = std::async(std::launch::async, &ListDirectory, entry);
			finalVect.push_back(std::move(fut));
		}
		else if (entry.is_regular_file())
		{

			vect.push_back(entry);
		}
	}

	std::for_each(finalVect.begin(), finalVect.end(), [&](std::future<std::vector<std::filesystem::directory_entry>>& fut)

		{
			vectDirEntry lst = fut.get();
			std::copy(lst.begin(), lst.end(), std::back_inserter(vect));
		}

	);
	return vect;
}


int main()
{

	const std::filesystem::directory_entry root = std::filesystem::directory_entry("C:/Test");
	std::future<std::vector<std::filesystem::directory_entry>> fut = std::async(std::launch::async, &ListDirectory, root);
	auto result = fut.get();

	for (std::filesystem::directory_entry& item : result)
	{

		std::cout << item << '\n';

	}
	return 0;
}
