## How to contribute

### Git-Related

#### **Did you find a bug?** / **Do you want to suggest something?**

- Create an issue at [this issue page](https://github.com/AndreasLrx/ecstasy/issues).

#### **Do you want to create a branch?**

- Your branch name should be formatted as `fix/<ISSUENUMBER>-<TITLE>` for bug fixes or `feature/<ISSUENUMBER>-<TITLE>` for features, example: `fix/4221-infinite-loop`.

#### **Do you want to fix an issue?**

- Create a branch

- Implement your features of fixes in it.

- Submit a [pull request](https://github.com/AndreasLrx/ecstasy/pulls).

- Once validated, merge to PR to `master` and remove the source branch (with `git branch -D <branch_name>`.

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

- You may add author command.

- The since command must be present.

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
  /// @throws std::logic_error Error condition.
  /// @author Andreas Leroux (andreas.leroux@epitech.eu)
  /// @since 0.1.0 (2022-07-09)
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
- Use UpperCamelCase for type names (classes, structs, enums, type aliases and template parameters):

  ```cpp
  template <typename ValueType>
  struct SuperSecretData {
      using ValuePtr = *ValueType;
      ValueType data;
  };
  ```

- Separate functions by an empty line (even for declaration in header files).

- Name header guards in the following format: [NAMESPACE0]\_[NAMESPACE1]\*\_FILENAME_HPP

---

### **DOs and DONTs**

- :x: **DONT**: Push to the `main` branch for any reason, please submit a PR instead.

- :x: **DONT**: Create a branch with your username as the title

- :heavy_check_mark: **DO**: Commit often! allows everyone to see your progress and eventually make suggestions on it.

- :heavy_check_mark: **DO**: Format your code, using either `clang-format` directly or your IDE's capabilities (and yes, VSCode can format your code for you!)

---

Thanks! :heart: :heart: :heart:
