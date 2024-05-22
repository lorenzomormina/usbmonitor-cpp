#pragma once

namespace lm {
namespace container {

template <typename Container>
bool contains(const Container &container,
              const typename Container::value_type &element) {
  return find(container.begin(), container.end(), element) != container.end();
}

} // namespace container
} // namespace lm