import './App.css';
import React, { useState } from 'react';
import WaterLevelTable from './WaterLevelChart';
import Login from './Login';

function App() {
  const [isLoggedIn, setIsLoggedIn] = useState(false);
  const [userEmail, setUserEmail] = useState(null);
  const [refreshTrigger, setRefreshTrigger] = useState(0);

  const handleLoginSuccess = (email) => {
    setIsLoggedIn(true);
    setUserEmail(email);
    setRefreshTrigger(prev => prev + 1);
  };

  const handleLogout = () => {
    setIsLoggedIn(false);
    setUserEmail(null);
    setRefreshTrigger(0);
  };

  const handleRefresh = () => {
    setRefreshTrigger(prev => prev + 1);
  };

  return (
    <div className="App">
      <header>
        <h1>Water Level Monitor</h1>
      </header>
      <main>
        {!isLoggedIn ? (
          <Login onLoginSuccess={handleLoginSuccess} />
        ) : (
          <>
            <div className="header-container">
              <h1>Water Level Monitor for {userEmail}</h1>
              <div className="header-buttons">
                <button onClick={handleRefresh} className="refresh-button">Refresh Data</button>
                <button onClick={handleLogout} className="logout-button">Logout</button>
              </div>
            </div>
            <WaterLevelTable userEmail={userEmail} refreshTrigger={refreshTrigger} />
          </>
        )}
      </main>
      <footer>
        <p>Water Level Monitor.</p>
      </footer>
    </div>
  );
}

export default App;
