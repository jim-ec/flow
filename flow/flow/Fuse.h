#pragma once

#include <optional>

namespace flow
{
    /// Ensures the further calls to `next()` on a sequence return `None`s
    /// after it has been returned once from the base sequence.
	/// Arity: 1 -> 1
    template<class Seq>
    class Fuse
    {
    public:
        // TODO: Optimized implementation for infinite flow?
        static inline bool constexpr finite = Seq::finite;
        using output_type = typename Seq::output_type;

        explicit Fuse(Seq const &base):
            base(base),
            exhausted(false)
        {}

        std::optional<output_type> next()
        {
            if (exhausted)
            {
                // The base sequence is already exhausted from previous calls
                // to this functional.
                return {};
            }

            std::optional<output_type> state(base.next());
            if (!state.has_value())
            {
                // The base sequence has been exhausted.
                exhausted = true;
                return {};
            }

            return state;
        }

    private:
        Seq base;
        bool exhausted;
    };

    auto fuse()
    {
        return [] (auto &&seq)
        {
            return Fuse(std::forward<decltype(seq)>(seq));
        };
    }
}
