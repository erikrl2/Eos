#pragma once

namespace Eos {

	class UID
	{
	public:
		UID();
		UID(uint64_t uid);
		UID(const UID&) = default;

		operator uint64_t() const { return m_UID; }
	private:
		uint64_t m_UID;
	};

}

namespace std {
	template <typename T> struct hash;

	template<>
	struct hash<Eos::UID>
	{
		uint64_t operator()(const Eos::UID& uid) const
		{
			return (uint64_t)uid;
		}
	};

}
