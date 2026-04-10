#+begin_src elisp

  ;; -----------------------------
  ;; UI / Editor Basics
  ;; -----------------------------
  (global-display-line-numbers-mode 1)
  (show-paren-mode 1)
  (electric-indent-mode 1)
  (setq-default indent-tabs-mode nil)

  ;; Auto-save before compile
  (setq compilation-ask-about-save nil)

  ;; -----------------------------
  ;; C Mode Configuration
  ;; -----------------------------
  (defun my-c-mode-setup ()
    (setq c-default-style "linux"
          c-basic-offset 4)
    (c-set-offset 'comment-intro 0))

  (add-hook 'c-mode-hook #'my-c-mode-setup)

  ;; -----------------------------
  ;; Autocomplete
  ;; -----------------------------
  (use-package company
    :ensure t
    :hook (after-init . global-company-mode))

  ;; -----------------------------
  ;; Syntax Checking (with AVR)
  ;; -----------------------------
  (use-package flycheck
    :ensure t
    :init (global-flycheck-mode))

  (flycheck-define-checker c-avr-gcc
    "AVR C syntax checker using avr-gcc."
    :command ("avr-gcc"
              "-mmcu=atmega328p"
              "-Wall"
              "-Os"
              "-fsyntax-only"
              source)
    :error-patterns
    ((error line-start (file-name) ":" line ":" column ": error: " (message) line-end)
     (warning line-start (file-name) ":" line ":" column ": warning: " (message) line-end))
    :modes c-mode)

  (add-to-list 'flycheck-checkers 'c-avr-gcc)

  ;; -----------------------------
  ;; LSP (clangd)
  ;; -----------------------------
  (use-package lsp-mode
    :hook ((c-mode c++-mode) . lsp)
    :commands lsp)

  (use-package lsp-ui
    :hook (lsp-mode . lsp-ui-mode))

  ;; -----------------------------
  ;; Compile / Build System
  ;; -----------------------------

  ;; Default: use Makefile (BEST for AVR projects)
  (setq compile-command "make -C ..")

  ;; Keybindings
  (global-set-key (kbd "<f5>") 'compile)


  ;; -----------------------------
  ;; AVR Flash (avrdude)
  ;; -----------------------------

  (defun avr-flash ()
    "Compile and flash AVR using avrdude."
    (interactive)
    (compile "make -C .. flash"))

  ;; Optional: direct avrdude command (if no Makefile)
  (defun avr-flash-direct ()
    "Flash using avrdude directly."
    (interactive)
    (compile
     "avrdude -c arduino -p m328p -P /dev/ttyUSB0 -b 115200 -U flash:w:main.hex"))

  ;; Keybinding for flashing
  (global-set-key (kbd "<f6>") 'avr-flash)

  ;; -----------------------------
  ;; Debugging
  ;; -----------------------------
  (use-package realgud
    :ensure t)

  ;; ----------------------------
  ;; Serial Monitor
  ;; ------------------------
  (defun avr-serial-monitor ()
    "Open serial monitor using picocom inside Emacs."
    (interactive)
    (let ((port "/dev/ttyUSB0")
          (baud "9600"))
      (term (concat "picocom -b " baud " " port))))

  (global-set-key (kbd "<f7>") 'avr-serial-monitor)


  ;; -----------------------------
  ;; AVR Debug Build (Step 3)
  ;; -----------------------------
  ;; (defun avr-build-debug ()
  ;;   "Compile AVR project with debug symbols."
  ;;   (interactive)
  ;;   (compile "avr-gcc -g -Og -DF_CPU=16000000UL -mmcu=atmega328p -Wall -o main.elf main.c"))
  (defun avr-build-debug ()
    "Build using Makefile debug target."
    (interactive)
    (compile "make -C .. debug"))
  
    (global-set-key (kbd "<f8>") 'avr-build-debug)



  ;; -----------------------------
  ;; Navigation (LSP)
  ;; -----------------------------
(with-eval-after-load 'lsp-mode
  (define-key lsp-mode-map (kbd "M-.") #'xref-find-definitions)
  (define-key lsp-mode-map (kbd "M-r") #'xref-find-references))

(use-package ggtags
  :ensure t
  :hook (c-mode . ggtags-mode))

#+end_src
