#include <string>
#include <iostream>
#include <filesystem>

typedef std::vector<std::filesystem::directory_entry> vectDirEntry;

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
