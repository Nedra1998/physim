#ifndef EXIT_CODE_HPP_3PDT5QSO
#define EXIT_CODE_HPP_3PDT5QSO

#define IF_OK(code, stmt) if (code == ExitCode::OK) code = (stmt);

enum ExitCode {
  OK = 0,
  INIT_LOG_ERROR,
};

#endif /* end of include guard: EXIT_CODE_HPP_3PDT5QSO */
