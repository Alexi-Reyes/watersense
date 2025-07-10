<?php

namespace App\Command;

use Symfony\Component\Console\Attribute\AsCommand;
use Symfony\Component\Console\Command\Command;
use Symfony\Component\Console\Input\InputArgument;
use Symfony\Component\Console\Input\InputInterface;
use Symfony\Component\Console\Input\InputOption;
use Symfony\Component\Console\Output\OutputInterface;
use Symfony\Component\Console\Style\SymfonyStyle;
use Symfony\Component\Mailer\MailerInterface;
use Symfony\Component\Mime\Email;
use Symfony\Component\Mime\Address; // Add this line
use App\Repository\WaterSenseRepository;
use App\Entity\User; // Assuming User entity exists and has an email property

#[AsCommand(
    name: 'send-warning',
    description: 'Sends a warning email if water level is too high',
)]
class SendWarningCommand extends Command
{
    private $waterSenseRepository;
    private $mailer;

    public function __construct(WaterSenseRepository $waterSenseRepository, MailerInterface $mailer)
    {
        $this->waterSenseRepository = $waterSenseRepository;
        $this->mailer = $mailer;
        parent::__construct();
    }

    protected function configure(): void
    {
        $this
            ->setDescription('Sends a warning email if water level is higher than 10.')
        ;
    }

    protected function execute(InputInterface $input, OutputInterface $output): int
    {
        $io = new SymfonyStyle($input, $output);

        $latestWaterSense = $this->waterSenseRepository->findLatest();

        if (!$latestWaterSense) {
            $io->warning('No water level data found.');
            return Command::SUCCESS;
        }

        $waterLevel = $latestWaterSense->getWaterLevel();
        $user = $latestWaterSense->getUser(); // Assuming WaterSense has a User association

        if ($waterLevel > 10) {
            $recipientEmail = 'admin@example.com'; // Default fallback email

            if ($user) {
                $userEmail = $user->getEmail();
                if ($userEmail !== null && $userEmail !== '') {
                    $recipientEmail = $userEmail;
                } else {
                    $io->warning('User email found but is empty or null for the latest water level entry. Sending to default admin email.');
                }
            } else {
                $io->warning('No user associated with the latest water level entry. Sending to default admin email.');
            }

            $email = (new Email())
                ->from('no-reply@watersense.com') // Revert to simple string for from
                ->to(new Address($recipientEmail)) // Keep explicit Address for to
                ->subject('WaterSense Warning: High Water Level Detected!')
                ->html('<p>The water level detected is <strong>' . $waterLevel . '</strong>, which is higher than the threshold of 10.</p><p>Please take appropriate action.</p>');

            try {
                $this->mailer->send($email);
                $io->success(sprintf('Warning email sent to %s for water level %d.', $recipientEmail, $waterLevel));
            } catch (\Exception $e) {
                $io->error('Failed to send email: ' . $e->getMessage());
                return Command::FAILURE;
            }
        } else {
            $io->info(sprintf('Water level is %d, which is within the normal range.', $waterLevel));
        }

        return Command::SUCCESS;
    }
}
