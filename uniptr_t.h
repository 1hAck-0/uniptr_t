#if !_UNIPTR_T_
#define _UNIPTR_T_ 1

#define _WINDOWS ((defined(WIN64) || _WIN64 || __WIN64 || defined(WIN32) || _WIN32 || __WIN32) && !__CYGWIN__)

#if _MSC_VER
#define ALWAYS_INLINE __forceinline
#else
#define ALWAYS_INLINE __attribute__((always_inline)) inline
#endif

#include <cstdint>
#if _WINDOWS
#include <Windows.h>
#endif

class uniptr_t
{
public:
	template<size_t __size> struct any_size_t { uint8_t data[__size]; };

	// CONSTRUCTORS
	ALWAYS_INLINE uniptr_t() : value(0) {}
	template<typename Ty>
	ALWAYS_INLINE uniptr_t(const Ty value) : value((uintptr_t)value) {}

	// SET
	template<size_t len>
	ALWAYS_INLINE void set(const void* src) { *(any_size_t<len>*)this->value = *(any_size_t<len>*)src; }
	template<typename Ty>
	ALWAYS_INLINE void set(const Ty value) { *(Ty*)this->value = value; }

	// GET
	template<size_t len>
	ALWAYS_INLINE any_size_t<len> get() const { return *(any_size_t<len>*)this->value; }
	template<typename Ty>
	ALWAYS_INLINE Ty get() const { return *(Ty*)this->value; }
	template<typename Ty>
	ALWAYS_INLINE Ty as() const { return (Ty)this->value; }
	template<typename Ty>
	ALWAYS_INLINE Ty* as_ptr() const { return (Ty*)this->value; }

	// OPERATORS
	template<typename Ty>
	ALWAYS_INLINE operator Ty() const { return (Ty)this->value; }
	template<typename Ty>
	ALWAYS_INLINE uniptr_t operator-(const Ty value) const { return uniptr_t(this->value - (uintptr_t)value); }
	template<typename Ty>
	ALWAYS_INLINE uniptr_t operator+(const Ty value) const { return uniptr_t(this->value + (uintptr_t)value); }
	template<typename Ty>
	ALWAYS_INLINE uniptr_t operator*(const Ty value) const { return uniptr_t(this->value * (uintptr_t)value); }
	template<typename Ty>
	ALWAYS_INLINE uniptr_t operator/(const Ty value) const { return uniptr_t(this->value / (uintptr_t)value); }
	template<typename Ty>
	ALWAYS_INLINE uniptr_t operator%(const Ty value) const { return uniptr_t(this->value % (uintptr_t)value); }
	template<typename Ty>
	ALWAYS_INLINE void operator+=(const Ty value) { this->value += (uintptr_t)value; }
	template<typename Ty>
	ALWAYS_INLINE void operator-=(const Ty value) { this->value -= (uintptr_t)value; }
	template<typename Ty>
	ALWAYS_INLINE void operator*=(const Ty value) { this->value *= (uintptr_t)value; }
	template<typename Ty>
	ALWAYS_INLINE void operator/=(const Ty value) { this->value /= (uintptr_t)value; }
	template<typename Ty>
	ALWAYS_INLINE void operator%=(const Ty value) { this->value %= (uintptr_t)value; }
	ALWAYS_INLINE void operator++() { ++this->value; }
	ALWAYS_INLINE void operator--() { --this->value; }
	ALWAYS_INLINE void operator++(int) { this->value++; }
	ALWAYS_INLINE void operator--(int) { this->value--; }

	// BITWISE OPERATORS
	template<typename Ty>
	ALWAYS_INLINE uniptr_t operator<<(const Ty value) const { return uniptr_t(this->value << (uintptr_t)value); }
	template<typename Ty>
	ALWAYS_INLINE void operator<<=(const Ty value) { this->value <<= (uintptr_t)value; }
	template<typename Ty>
	ALWAYS_INLINE uniptr_t operator>>(const Ty value) const { return uniptr_t(this->value >> (uintptr_t)value); }
	template<typename Ty>
	ALWAYS_INLINE void operator>>=(const Ty value) { this->value >>= (uintptr_t)value; }
	template<typename Ty>
	ALWAYS_INLINE uniptr_t operator&(const Ty value) const { return uniptr_t(this->value & (uintptr_t)value); }
	template<typename Ty>
	ALWAYS_INLINE void operator&=(const Ty value) { this->value &= (uintptr_t)value; }
	template<typename Ty>
	ALWAYS_INLINE uniptr_t operator|(const Ty value) const { return uniptr_t(this->value | (uintptr_t)value); }
	template<typename Ty>
	ALWAYS_INLINE void operator|=(const Ty value) { this->value |= (uintptr_t)value; }
	template<typename Ty>
	ALWAYS_INLINE uniptr_t operator^(const Ty value) const { return uniptr_t(this->value ^ (uintptr_t)value); }
	template<typename Ty>
	ALWAYS_INLINE void operator^=(const Ty value) { this->value ^= (uintptr_t)value; }
	ALWAYS_INLINE uniptr_t operator~() const { return uniptr_t(~this->value); }

	// COMPARISON OPERATORS
	ALWAYS_INLINE bool operator!() const { return !this->value; }
	template<typename Ty>
	ALWAYS_INLINE bool operator==(const Ty value) const { return this->value == (uintptr_t)value; }
	template<typename Ty>
	ALWAYS_INLINE bool operator!=(const Ty value) const { return this->value != (uintptr_t)value; }
	template<typename Ty>
	ALWAYS_INLINE bool operator<(const Ty value) const { return this->value < (uintptr_t)value; }
	template<typename Ty>
	ALWAYS_INLINE bool operator<=(const Ty value) const { return this->value <= (uintptr_t)value; }
	template<typename Ty>
	ALWAYS_INLINE bool operator>(const Ty value) const { return this->value > (uintptr_t)value; }
	template<typename Ty>
	ALWAYS_INLINE bool operator>=(const Ty value) const { return this->value >= (uintptr_t)value; }

	// UTILITY
	static constexpr ALWAYS_INLINE size_t size() { return sizeof(void*); }

	template<typename Ty = uint32_t>
	ALWAYS_INLINE uniptr_t relative_addr(size_t offsetToRead, size_t instructionLen) const
	{
		return this->value + instructionLen + *(Ty*)(this->value + offsetToRead);
	}

	template<typename Ty = uint32_t>
	ALWAYS_INLINE void to_relative_addr(size_t offsetToRead, size_t instructionLen)
	{
		this->value += instructionLen + *(Ty*)(this->value + offsetToRead);
	}

#if _WINDOWS

	__declspec(noinline) bool is_valid(size_t szRegionMinSize = sizeof(void*)) const
	{
		MEMORY_BASIC_INFORMATION mbi;
		return VirtualQuery((void*)this->value, &mbi, sizeof(mbi)) &&
			mbi.RegionSize >= szRegionMinSize &&
			mbi.State & MEM_COMMIT &&
			mbi.Protect;
	}

	__declspec(noinline) bool is_readable(size_t szRegionMinSize = sizeof(void*)) const
	{
		MEMORY_BASIC_INFORMATION mbi;
		return VirtualQuery((void*)this->value, &mbi, sizeof(mbi)) &&
			mbi.RegionSize >= szRegionMinSize &&
			mbi.State & MEM_COMMIT &&
			mbi.Protect & (
				PAGE_READONLY | PAGE_READWRITE |
				PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE |
				PAGE_GRAPHICS_READONLY | PAGE_GRAPHICS_READWRITE |
				PAGE_GRAPHICS_EXECUTE_READ | PAGE_GRAPHICS_EXECUTE_READWRITE);
	}

	__declspec(noinline) bool is_writeable(size_t szRegionMinSize = sizeof(void*)) const
	{
		MEMORY_BASIC_INFORMATION mbi;
		return VirtualQuery((void*)this->value, &mbi, sizeof(mbi)) &&
			mbi.RegionSize >= szRegionMinSize &&
			mbi.State & MEM_COMMIT &&
			mbi.Protect & (
				PAGE_WRITECOPY | PAGE_READWRITE |
				PAGE_EXECUTE_WRITECOPY | PAGE_EXECUTE_READWRITE |
				PAGE_GRAPHICS_READWRITE | PAGE_GRAPHICS_EXECUTE_READWRITE);
	}

	__declspec(noinline) size_t region_size() const
	{
		MEMORY_BASIC_INFORMATION mbi;
		return VirtualQuery((void*)this->value, &mbi, sizeof(mbi)) ? mbi.RegionSize : 0;
	}

#endif // _WINDOWS

private:
	uintptr_t value;
};

#undef ALWAYS_INLINE
#undef _WINDOWS
#endif // _UNIPTR_T_
