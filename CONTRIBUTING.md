## How to contribute

### Git-Related

#### **Did you find a bug?** / **Do you want to suggest something?**

- Create an issue at [this issue page](https://github.com/AndreasLrx/ecstasy/issues).

#### **Do you want to create a branch?**

- You need to work on your own [fork](https://docs.github.com/articles/fork-a-repo), unless you are added as a collaborator (in case you are a recurrent contributor)

- Your branch name should be formatted as `fix/<ISSUENUMBER>-<TITLE>` for bug fixes or `feature/<ISSUENUMBER>-<TITLE>` for features, example: `fix/4221-infinite-loop`.

#### **Rebase or Merge strategy ?**

- Use the rebase strategy instead of the merge.

- PR with merge commits will not be validated.

- The project began by using merge commit, which explains the presence of old merge commits.

#### **Do you want to fix an issue?**

- Create a branch

- Implement your features of fixes in it.

- Submit a [pull request](https://github.com/AndreasLrx/ecstasy/pulls).

- Once validated, rebase and merge to PR to `main` and remove the source branch (with `git branch -D <branch_name>`).

#### **How to title commits?**

- Follow the guidelines at https://cbea.ms/git-commit/

- Use imperative tense (avoid past tense).

- The title of the commit must be a summuary of the content and not be too long (less than 50 characters).

- Prefer putting detailed informations inside the commit's description.

- Example:

  ```sh
  $> git commit -m 'Fix infinite loop when pressing Alt-F4

  This was caused by a missing check in the event loop
  The program now checks when the window is set to close'
  ```

---

### Code Documentation

[Doxygen](https://doxygen.nl/) is the tool of choice for code documentation the format to use for in-code documentation is as follows:

- Doc comments are signaled by triple-slashes (`///`) for each line.

- Doxygen commands are written javadoc-style, using `@` instead of `\` (e.g. `@param`).

- Documented items should have a sentence (may span more than a line) that quickly describes it.
  It is called a `brief` in Doxygen terms, the `@brief` command that would be present here shall not be omitted.

- The brief line should be separated from the rest of the doc comment by an empty comment.

- You may add `@author` command.

- The `@since` command must be present.

- You need to add the `@throw` command if your function may raise exceptions.

- You should also add the `@throw` command if the underlying functions may raise, but it is not mandatory especially if it is very uncommon (such as @ref std::bad_alloc)

- Example:
  ```cpp
  ///
  /// @brief Short brief line.
  ///
  /// More in-depth description (optional)
  ///
  /// @param parameter1 Parameter description.
  ///
  /// @returns Some value
  ///
  /// @throws std::logic_error Error condition.
  ///
  /// @author Andreas Leroux (andreas.leroux@epitech.eu)
  /// @since 1.0.0 (2022-07-09)
  ///
  int someFunction(int parameter1);
  ```

---

### Coding Style

- The format to use is defined in the [.clang_format](https://github.com/AndreasLrx/ecstasy/blob/main/.clang-format) file at the root of the project.

- Use lowerCamelCase for function names and non-constant variables.

- Use SCREAMING_CASE for constants:
  ```cpp
  constexpr int MEANING_OF_LIFE = 42;
  ```
- Use UpperCamelCase for type names (classes, structs, enums, type aliases, template parameters and concepts):

  ```cpp
  template <typename ValueType>
  struct SuperSecretData {
      using ValuePtr = *ValueType;
      ValueType data;
  };
  ```

- Use snake_case for [type_traits](https://en.cppreference.com/w/cpp/header/type_traits) or other meta structures

- Separate functions by an empty line (even for declaration in header files).

- Name header guards in the following format: [NAMESPACE0]\_[NAMESPACE1]\*\_FILENAME_HPP

#### Using attributes specifiers and keywords

These are not always mandatory, but having it in mind result in free compiler optimizations (likely, noexcept...) and avoid some useless code (discarding the return value of a getter).

1. [Attributes specifiers](https://en.cppreference.com/w/cpp/language/attributes):
   - [[[maybe_unused]]](https://en.cppreference.com/w/cpp/language/attributes/maybe_unused): Use for unused function parameters instead of using static_cast<void>.
   - [[[nodiscard]]](https://en.cppreference.com/w/cpp/language/attributes/nodiscard): For every function whose purpose is to return a value (instead of inplace edition). This explicitly includes all getters.
   - [[[(un)likely]]](https://en.cppreference.com/w/cpp/language/attributes/likely): For branch conditions which are not taken evenly. Checks before raising exceptions are usually [[unlikely]] (otherwise it would not be called an exception)
2. Other keywords:
   - [noexcept](https://en.cppreference.com/w/cpp/language/noexcept_spec): For all functions **never** raising an exception.
   - [final](https://en.cppreference.com/w/cpp/language/final): For all overriding objects (class, methods...) which mustn't be overriden after.

---

### **DOs and DONTs**

- :x: **DONT**: Push to the `main` branch for any reason, please submit a PR instead.

- :x: **DONT**: Create a branch with your username as the title

- :heavy_check_mark: **DO**: Commit often! allows everyone to see your progress and eventually make suggestions on it.

- :heavy_check_mark: **DO**: Format your code, using either `clang-format` directly or your IDE's capabilities (and yes, VSCode can format your code for you!)

---

Thanks! :heart: :heart: :heart:
