
# TODOs

 * Current json library is slow and requires large amounts of memory on large files.
   Look for faster json parser.<br>
   Might be difficult. Very fast libraries are not suitable.<br>
   Maybe write you own parser?

 * Add configuration struct to map parser. To configure things like:
    * Abort on error
    * Verbosity of print messages
    * Handling of incomplete elements

 * Use tracy for performance profiling

 * Handle error when file could not be opened.