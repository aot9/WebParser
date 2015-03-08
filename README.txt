=========================
Features
-------------------------
    * Start/Pause/Resume button
    * Stop button
    * Customizible text to search, start url, number of threads and number
      of links to scan
    * Progress bar showing number/percent of links scanned so far
    * Display total number of mathces
    * Display link with maximum number of matches

=========================
Notes
-------------------------
    * Number of threads is limited by 9 and max. number of links to scan
      is limited by 999. Bigger numbers seem to be not reasonable
    * Pausing may take some time(waiting for tasks that currently in progress)
    * Links that had been processed with errors are displayed in red color

=========================
Possible testing workflow
-------------------------
    * Check interaction with GUI. Sure that inappropriate user input won't crash
      the app
    * Internal module testing
    * Performance testing. Check processor time and memory usage to be at 
      reasonable rates

