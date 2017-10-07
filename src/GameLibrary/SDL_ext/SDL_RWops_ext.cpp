
#include "SDL_RWops_ext.hpp"
#include <limits>

Sint64 SDL_RWFILE_size(SDL_RWops* ops)
{
	FILE* file = ops->hidden.stdio.fp;
	if(file==nullptr)
	{
		SDL_SetError("FILE has not been opened");
		return -1;
	}
	//get original position
	long pos = std::ftell(file);
	if(pos==-1L)
	{
		return -1;
	}
	//go to the end of the file
	int sought = std::fseek(file, 0, SEEK_END);
	if(sought != 0)
	{
		std::fseek(file, pos, SEEK_SET);
		return -1;
	}
	//size of the file is the position at the end
	long size = std::ftell(file);
	//seek back to original position
	std::fseek(file, pos, SEEK_SET);
	return size;
}

Sint64 SDL_RWFILE_seek(SDL_RWops* ops, Sint64 offset, int whence)
{
	int origin = 0;
	switch(whence)
	{
		case RW_SEEK_SET:
			origin = SEEK_SET;
			break;

		case RW_SEEK_CUR:
			origin = SEEK_CUR;
			break;

		case RW_SEEK_END:
			origin = SEEK_END;
			break;

		default:
			SDL_SetError("invalid origin");
			return -1;
	}

	if(offset > (Sint64)std::numeric_limits<long>::max)
	{
		SDL_SetError("offset is too large");
		return -1;
	}

	FILE* file = ops->hidden.stdio.fp;
	if(file==nullptr)
	{
		SDL_SetError("FILE has not been opened");
		return -1;
	}
	return (Sint64)std::fseek(file, (long)offset, origin);
}

size_t SDL_RWFILE_read(SDL_RWops* ops, void* ptr, size_t size, size_t maxnum)
{
	FILE* file = ops->hidden.stdio.fp;
	if(file==nullptr)
	{
		SDL_SetError("FILE has not been opened");
		return 0;
	}
	size_t numRead = std::fread(ptr, size, maxnum, file);
	if(numRead==0 && ferror(file))
	{
		SDL_Error(SDL_EFREAD);
	}
	return numRead;
}

size_t SDL_RWFILE_write(SDL_RWops* ops, const void* ptr, size_t size, size_t num)
{
	FILE* file = ops->hidden.stdio.fp;
	if(file==nullptr)
	{
		SDL_SetError("FILE has not been opened");
		return 0;
	}
	size_t numWritten = std::fwrite(ptr, size, num, file);
	if(numWritten==0 && ferror(file))
	{
		SDL_Error(SDL_EFWRITE);
	}
	return numWritten;
}

int SDL_RWFILE_close(SDL_RWops* ops)
{
	int status = 0;
	if(ops!=nullptr)
	{
		FILE* file = ops->hidden.stdio.fp;
		if(file!=nullptr)
		{
			if(ops->hidden.stdio.autoclose)
			{
				if(std::fclose(file) != 0)
				{
					status = SDL_Error(SDL_EFWRITE);
				}
			}
		}
		SDL_FreeRW(ops);
	}
	return status;
}

SDL_RWops* SDL_RWFromFILE(FILE* file, SDL_bool autoclose)
{
	SDL_RWops* ops = SDL_AllocRW();
	if(ops != nullptr)
	{
		ops->size = SDL_RWFILE_size;
		ops->seek = SDL_RWFILE_seek;
		ops->read = SDL_RWFILE_read;
		ops->write = SDL_RWFILE_write;
		ops->close = SDL_RWFILE_close;
		ops->type = SDL_RWOPS_STDFILE;
		ops->hidden.stdio.fp = file;
		ops->hidden.stdio.autoclose = autoclose;
	}
	return ops;
}
